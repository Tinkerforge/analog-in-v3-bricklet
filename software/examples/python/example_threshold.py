#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Analog In Bricklet 3.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_analog_in_v3 import BrickletAnalogInV3

# Callback function for voltage callback (parameter has unit mV)
def cb_voltage(voltage):
    print("Voltage: " + str(voltage/1000.0) + " V")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ai = BrickletAnalogInV3(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    ai.set_debounce_period(10000)

    # Register voltage callback to function cb_voltage
    ai.register_callback(ai.CALLBACK_VOLTAGE, cb_voltage)

    # Configure threshold for voltage "outside of 5 to 0 V" (unit is mV)
    # with a debounce period of 1s (1000ms)
    ai.set_voltage_callback_configuration(1000, False, "o", 5*1000, 0)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
