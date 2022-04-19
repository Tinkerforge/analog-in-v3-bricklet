// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_analog_in_v3.h"

#define UID "XYZ" // Change XYZ to the UID of your Analog In Bricklet 3.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


static TF_AnalogInV3 ai;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_analog_in_v3_create(&ai, UID, hal), "create device object");

	// Get current voltage
	uint16_t voltage;
	check(tf_analog_in_v3_get_voltage(&ai, &voltage), "get voltage");

	tf_hal_printf("Voltage: %d 1/%d V\n", voltage, 1000);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
