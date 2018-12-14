package main

import (
	"fmt"
	"tinkerforge/analog_in_v3_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Analog In Bricklet 3.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	ai, _ := analog_in_v3_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	ai.RegisterVoltageCallback(func(voltage uint16) {
		fmt.Printf("Voltage: %f V\n", float64(voltage)/1000.0)
	})

	// Set period for voltage callback to 1s (1000ms) without a threshold.
	ai.SetVoltageCallbackConfiguration(1000, false, 'x', 0, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
