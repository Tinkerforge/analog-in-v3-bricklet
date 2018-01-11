#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Analog In Bricklet 3.0

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call analog-in-v3-bricklet $uid set-debounce-period 10000

# Handle incoming voltage callbacks (parameter has unit mV)
tinkerforge dispatch analog-in-v3-bricklet $uid voltage &

# Configure threshold for voltage "outside of 5 to 0 V" (unit is mV)
# with a debounce period of 1s (1000ms)
tinkerforge call analog-in-v3-bricklet $uid set-voltage-callback-configuration 1000 false threshold-option-outside 5000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
