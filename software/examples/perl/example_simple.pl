#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletAnalogInV3;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Analog In Bricklet 3.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ai = Tinkerforge::BrickletAnalogInV3->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current voltage (unit is mV)
my $voltage = $ai->get_voltage();
print "Voltage: " . $voltage/1000.0 . " V\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
