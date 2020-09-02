// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_analog_in_v3.h"

#define UID "XYZ" // Change XYZ to the UID of your Analog In Bricklet 3.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for voltage callback
static void voltage_handler(TF_AnalogInV3 *device, uint16_t voltage, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Voltage: %d 1/%d V\n", voltage, 1000.0);
}

static TF_AnalogInV3 ai;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_analog_in_v3_create(&ai, UID, hal), "create device object");

	// Register voltage callback to function voltage_handler
	tf_analog_in_v3_register_voltage_callback(&ai,
	                                          voltage_handler,
	                                          NULL);

	// Set period for voltage callback to 1s (1000ms) without a threshold
	tf_analog_in_v3_set_voltage_callback_configuration(&ai, 1000, false, 'x', 0, 0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
