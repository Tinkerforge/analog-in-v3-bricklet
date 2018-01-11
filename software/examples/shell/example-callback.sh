#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Analog In Bricklet 3.0

# Handle incoming voltage callbacks (parameter has unit mV)
tinkerforge dispatch analog-in-v3-bricklet $uid voltage &

# Set period for voltage callback to 1s (1000ms) without a threshold
tinkerforge call analog-in-v3-bricklet $uid set-voltage-callback-configuration 1000 false threshold-option-off 0 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
