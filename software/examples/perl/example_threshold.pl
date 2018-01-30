#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletAnalogInV3;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Analog In Bricklet 3.0

# Callback subroutine for voltage callback
sub cb_voltage
{
    my ($voltage) = @_;

    print "Voltage: " . $voltage/1000.0 . " V\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ai = Tinkerforge::BrickletAnalogInV3->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register voltage callback to subroutine cb_voltage
$ai->register_callback($ai->CALLBACK_VOLTAGE, 'cb_voltage');

# Configure threshold for voltage "smaller than 5 V"
# with a debounce period of 1s (1000ms)
$ai->set_voltage_callback_configuration(1000, 0, '<', 5*1000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
