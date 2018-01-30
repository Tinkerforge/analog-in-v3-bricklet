#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Analog In Bricklet 3.0

# Get current voltage
tinkerforge call analog-in-v3-bricklet $uid get-voltage
