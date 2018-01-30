#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_analog_in_v3'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Analog In Bricklet 3.0

ipcon = IPConnection.new # Create IP connection
ai = BrickletAnalogInV3.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register voltage callback
ai.register_callback(BrickletAnalogInV3::CALLBACK_VOLTAGE) do |voltage|
  puts "Voltage: #{voltage/1000.0} V"
end

# Configure threshold for voltage "smaller than 5 V"
# with a debounce period of 1s (1000ms)
ai.set_voltage_callback_configuration 1000, false, '<', 5*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
