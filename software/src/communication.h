/* analog-in-v3-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define ANALOG_IN_V3_THRESHOLD_OPTION_OFF 'x'
#define ANALOG_IN_V3_THRESHOLD_OPTION_OUTSIDE 'o'
#define ANALOG_IN_V3_THRESHOLD_OPTION_INSIDE 'i'
#define ANALOG_IN_V3_THRESHOLD_OPTION_SMALLER '<'
#define ANALOG_IN_V3_THRESHOLD_OPTION_GREATER '>'

#define ANALOG_IN_V3_OVERSAMPLING_32 0
#define ANALOG_IN_V3_OVERSAMPLING_64 1
#define ANALOG_IN_V3_OVERSAMPLING_128 2
#define ANALOG_IN_V3_OVERSAMPLING_256 3
#define ANALOG_IN_V3_OVERSAMPLING_512 4
#define ANALOG_IN_V3_OVERSAMPLING_1024 5
#define ANALOG_IN_V3_OVERSAMPLING_2048 6
#define ANALOG_IN_V3_OVERSAMPLING_4096 7
#define ANALOG_IN_V3_OVERSAMPLING_8192 8
#define ANALOG_IN_V3_OVERSAMPLING_16384 9

#define ANALOG_IN_V3_BOOTLOADER_MODE_BOOTLOADER 0
#define ANALOG_IN_V3_BOOTLOADER_MODE_FIRMWARE 1
#define ANALOG_IN_V3_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define ANALOG_IN_V3_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define ANALOG_IN_V3_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define ANALOG_IN_V3_BOOTLOADER_STATUS_OK 0
#define ANALOG_IN_V3_BOOTLOADER_STATUS_INVALID_MODE 1
#define ANALOG_IN_V3_BOOTLOADER_STATUS_NO_CHANGE 2
#define ANALOG_IN_V3_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define ANALOG_IN_V3_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define ANALOG_IN_V3_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define ANALOG_IN_V3_STATUS_LED_CONFIG_OFF 0
#define ANALOG_IN_V3_STATUS_LED_CONFIG_ON 1
#define ANALOG_IN_V3_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define ANALOG_IN_V3_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_VOLTAGE 1
#define FID_SET_VOLTAGE_CALLBACK_CONFIGURATION 2
#define FID_GET_VOLTAGE_CALLBACK_CONFIGURATION 3
#define FID_SET_OVERSAMPLING 5
#define FID_GET_OVERSAMPLING 6
#define FID_SET_CALIBRATION 7
#define FID_GET_CALIBRATION 8

#define FID_CALLBACK_VOLTAGE 4

typedef struct {
	TFPMessageHeader header;
	uint8_t oversampling;
} __attribute__((__packed__)) SetOversampling;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetOversampling;

typedef struct {
	TFPMessageHeader header;
	uint8_t oversampling;
} __attribute__((__packed__)) GetOversampling_Response;

typedef struct {
	TFPMessageHeader header;
	int16_t offset;
	uint16_t multiplier;
	uint16_t divisor;
} __attribute__((__packed__)) SetCalibration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCalibration;

typedef struct {
	TFPMessageHeader header;
	int16_t offset;
	uint16_t multiplier;
	uint16_t divisor;
} __attribute__((__packed__)) GetCalibration_Response;


// Function prototypes
BootloaderHandleMessageResponse set_oversampling(const SetOversampling *data);
BootloaderHandleMessageResponse get_oversampling(const GetOversampling *data, GetOversampling_Response *response);
BootloaderHandleMessageResponse set_calibration(const SetCalibration *data);
BootloaderHandleMessageResponse get_calibration(const GetCalibration *data, GetCalibration_Response *response);

// Callbacks
bool handle_voltage_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 1
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_voltage_callback, \


#endif
