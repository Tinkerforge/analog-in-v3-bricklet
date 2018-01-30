<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAnalogInV3.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAnalogInV3;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Analog In Bricklet 3.0

$ipcon = new IPConnection(); // Create IP connection
$ai = new BrickletAnalogInV3(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current voltage
$voltage = $ai->getVoltage();
echo "Voltage: " . $voltage/1000.0 . " V\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
