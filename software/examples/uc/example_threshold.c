// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_analog_in_v3.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Callback function for voltage callback
static void voltage_handler(TF_AnalogInV3 *device, uint16_t voltage, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Voltage: %d 1/%d V\n", voltage, 1000);
}

static TF_AnalogInV3 ai;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_analog_in_v3_create(&ai, NULL, hal), "create device object");

	// Register voltage callback to function voltage_handler
	tf_analog_in_v3_register_voltage_callback(&ai,
	                                          voltage_handler,
	                                          NULL);

	// Configure threshold for voltage "smaller than 5 V"
	// with a debounce period of 1s (1000ms)
	tf_analog_in_v3_set_voltage_callback_configuration(&ai, 1000, false, '<', 5*1000, 0);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
