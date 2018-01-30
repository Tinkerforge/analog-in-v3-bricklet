#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Analog In Bricklet 3.0

# Handle incoming voltage callbacks
tinkerforge dispatch analog-in-v3-bricklet $uid voltage &

# Configure threshold for voltage "smaller than 5 V"
# with a debounce period of 1s (1000ms)
tinkerforge call analog-in-v3-bricklet $uid set-voltage-callback-configuration 1000 false threshold-option-smaller 5000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
