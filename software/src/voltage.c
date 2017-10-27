/* analog-in-v3-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * voltage.c: Calculate voltage from ADC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "voltage.h"

#include <string.h>

#include "configs/config.h"
#include "configs/config_voltage.h"

#include "xmc_vadc.h"
#include "xmc_gpio.h"

#include "communication.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/hal/uartbb/uartbb.h"
#include "bricklib2/utility/util_definitions.h"

#define VOLTAGE_BUFFER_SIZE_MAX 4096

#define voltage_adc_irq_handler IRQ_Hdlr_15


// Keep the variables that are used in the interrupts as simple global variables,
// to be sure that the compiler does not need to do any derefernciation or similar.
static uint16_t voltage_buffer[VOLTAGE_BUFFER_SIZE_MAX];
static volatile uint32_t voltage_buffer_index = 0;
static uint32_t voltage_buffer_size = (1 << (ANALOG_IN_V3_OVERSAMPLING_4096 + 3));
static uint32_t voltage_buffer_mask = (1 << (ANALOG_IN_V3_OVERSAMPLING_4096 + 3))-1;
static uint32_t voltage_sum = 0;

Voltage voltage;

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) voltage_adc_irq_handler(void) {
	// Remove oldest value from moving average
	voltage_sum -= voltage_buffer[voltage_buffer_index];

	// Save new value to buffer and add it to moving average
	const uint16_t new_value = VADC->GLOBRES & 0xFFFF;
	voltage_buffer[voltage_buffer_index] = new_value;
	voltage_sum += new_value;

	// Increase index
	voltage_buffer_index = (voltage_buffer_index + 1) & voltage_buffer_mask;
}

void voltage_calibration_write(void) {
	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];

	page[VOLTAGE_CALIBRATION_MAGIC_POS]      = VOLTAGE_CALIBRATION_MAGIC;
	page[VOLTAGE_CALIBRATION_OFFSET_POS]     = voltage.calibration_offset - INT16_MIN;
	page[VOLTAGE_CALIBRATION_MULTIPLIER_POS] = voltage.calibration_multiplier;
	page[VOLTAGE_CALIBRATION_DIVISOR_POS]    = voltage.calibration_divisor;

	bootloader_write_eeprom_page(VOLTAGE_CALIBRATION_PAGE, page);
}

void voltage_calibration_read(void) {
	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];
	bootloader_read_eeprom_page(VOLTAGE_CALIBRATION_PAGE, page);

	// The magic number is not where it is supposed to be.
	// This is either our first startup or something went wrong.
	// We initialize the calibration data with sane default values.
	if(page[0] != VOLTAGE_CALIBRATION_MAGIC) {
		voltage.calibration_offset     = 0;
		voltage.calibration_multiplier = 1;
		voltage.calibration_divisor    = 1;
		voltage_calibration_write();
		return;
	}

	voltage.calibration_offset     = page[VOLTAGE_CALIBRATION_OFFSET_POS] + INT16_MIN;
	voltage.calibration_multiplier = page[VOLTAGE_CALIBRATION_MULTIPLIER_POS];
	voltage.calibration_divisor    = page[VOLTAGE_CALIBRATION_DIVISOR_POS];
}

void voltage_oversampling_update(void) {
	NVIC_DisableIRQ(VOLTAGE_ADC_IRQ);

	// Use old oversampling settings to get current average value
	uint32_t sum = 0;
	for(uint16_t i = 0; i < voltage_buffer_size; i++) {
		sum += voltage_buffer[i];
	}

	// Save value
	uint16_t current_value = sum/voltage_buffer_size;

	// Apply new oversampling settings
	voltage_buffer_size = (1 << (voltage.oversampling+3));
	voltage_buffer_mask = voltage_buffer_size - 1;
	voltage_buffer_index = 0;

	// Set complete buffer to saved value. This way we don't introduce any gaps in the data.
	for(uint16_t i = 0; i < voltage_buffer_size; i++) {
		voltage_buffer[i] = current_value;
	}
	voltage_sum = current_value*voltage_buffer_size;

	NVIC_EnableIRQ(VOLTAGE_ADC_IRQ);
}

void voltage_init_adc(void) {
	// This structure contains the Global related Configuration.
	const XMC_VADC_GLOBAL_CONFIG_t adc_global_config = {
		.boundary0 = (uint32_t) 0, // Lower boundary value for Normal comparison mode
		.boundary1 = (uint32_t) 0, // Upper boundary value for Normal comparison mode

		.class0 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},
		.class1 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},

		.data_reduction_control         = 0b11, // Accumulate 4 result values
		.wait_for_read_mode             = 0, // GLOBRES Register will not be overwritten until the previous value is read
		.event_gen_enable               = 1, // Result Event from GLOBRES is enabled
		.disable_sleep_mode_control     = 0  // Sleep mode is enabled
	};


	// Global iclass0 configuration
	const XMC_VADC_GLOBAL_CLASS_t adc_global_iclass_config = {
		.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
		.sample_time_std_conv	  = 31,
	};

	// Global Result Register configuration structure
	XMC_VADC_RESULT_CONFIG_t adc_global_result_config = {
		.data_reduction_control = 0b11, // Accumulate 4 result values
		.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
		.wait_for_read_mode  	= 1, // Enabled
		.part_of_fifo       	= 0, // No FIFO
		.event_gen_enable   	= 1  // Enable Result event
	};

	// LLD Background Scan Init Structure
	const XMC_VADC_BACKGROUND_CONFIG_t adc_background_config = {
		.conv_start_mode   = XMC_VADC_STARTMODE_CIR,       // Conversion start mode selected as cancel inject repeat
		.req_src_priority  = XMC_VADC_GROUP_RS_PRIORITY_1, // Priority of the Background request source in the VADC module
		.trigger_signal    = XMC_VADC_REQ_TR_A,            // If Trigger needed then this denotes the Trigger signal
		.trigger_edge      = XMC_VADC_TRIGGER_EDGE_NONE,   // If Trigger needed then this denotes Trigger edge selected
		.gate_signal       = XMC_VADC_REQ_GT_A,			   // If Gating needed then this denotes the Gating signal
		.timer_mode        = 0,							   // Timer Mode Disabled
		.external_trigger  = 0,                            // Trigger is Disabled
		.req_src_interrupt = 0,                            // Background Request source interrupt Disabled
		.enable_auto_scan  = 1,
		.load_mode         = XMC_VADC_SCAN_LOAD_OVERWRITE
	};


	XMC_VADC_GLOBAL_Init(VADC, &adc_global_config);
	XMC_VADC_GLOBAL_StartupCalibration(VADC);

	// Initialize the Global Conversion class 0
	XMC_VADC_GLOBAL_InputClassInit(VADC, adc_global_iclass_config, XMC_VADC_GROUP_CONV_STD, 0);
	// Initialize the Global Conversion class 1
	XMC_VADC_GLOBAL_InputClassInit(VADC, adc_global_iclass_config, XMC_VADC_GROUP_CONV_STD, 1);

	// Initialize the Background Scan hardware
	XMC_VADC_GLOBAL_BackgroundInit(VADC, &adc_background_config);

	// Initialize the global result register
	XMC_VADC_GLOBAL_ResultInit(VADC, &adc_global_result_config);

	XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(VADC, 0, VOLTAGE_ADC_CHANNEL);
	XMC_VADC_GLOBAL_SetResultEventInterruptNode(VADC, XMC_VADC_SR_SHARED_SR0);

	XMC_VADC_GLOBAL_BackgroundTriggerConversion(VADC);

    NVIC_SetPriority(VOLTAGE_ADC_IRQ, VOLTAGE_ADC_IRQ_PRIORITY);
    NVIC_EnableIRQ(VOLTAGE_ADC_IRQ);
}

void voltage_init(void) {
	const XMC_GPIO_CONFIG_t voltage_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE,
	};
	XMC_GPIO_Init(VOLTAGE_ADC_PIN, &voltage_pin_config);

	memset(&voltage, 0, sizeof(Voltage));
	voltage.calibration_divisor    = 1;
	voltage.calibration_multiplier = 1;
	voltage.calibration_offset     = 0;
	voltage.oversampling           = ANALOG_IN_V3_OVERSAMPLING_4096;

	voltage_calibration_read();

	voltage_init_adc();

	// Wait for first adc data to arrive and fill buffer with value.
	// Otherwise it would take ~50ms until we get meaningful data.
	while(voltage_buffer_index == 0);
	NVIC_DisableIRQ(VOLTAGE_ADC_IRQ);
	for(uint16_t i = 1; i < voltage_buffer_size; i++) {
		voltage_buffer[i] = voltage_buffer[0];
	}
	voltage_sum = voltage_buffer[0]*voltage_buffer_size;

	NVIC_EnableIRQ(VOLTAGE_ADC_IRQ);
}

void voltage_tick(void) {
	if(voltage.calibration_new) {
		voltage_calibration_write();
		voltage.calibration_new = false;
	}

	if(voltage.oversampling_new) {
		voltage_oversampling_update();
		voltage.oversampling_new = false;
	}
}

uint16_t voltage_get_voltage(void) {
	// Max voltage_sum is 4095*4 (14 bit) * 4096 (12 bit) = 26 bit

	// To go from sum to value we have to multiply by 3300 (3.3V processor voltage) and 13 (opamp multiplier).
	// We have to divide by 4 (adc oversampling) and 4095 (12 bit adc) and voltage_buffer_size (software oversampling)

	// This reduces to 55/21 (6 bit multiplier), so it fits in 26+6 = 32 bit!
	const uint32_t value = (voltage_sum*55ULL)/(21*voltage_buffer_size);

	// Now we can apply the calibration. This can become negative because of the offset
	// Since calibration and value are at max 16 bit each, it should always fit in the int32.
	const int32_t calibrated_value = (value+voltage.calibration_offset)*voltage.calibration_multiplier/voltage.calibration_divisor;

	// We never return negative or out-of-bounds values
	return BETWEEN(0, calibrated_value, UINT16_MAX);
}
