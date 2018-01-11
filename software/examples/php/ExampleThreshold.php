<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAnalogInV3.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAnalogInV3;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Analog In Bricklet 3.0

// Callback function for voltage callback (parameter has unit mV)
function cb_voltage($voltage)
{
    echo "Voltage: " . $voltage/1000.0 . " V\n";
}

$ipcon = new IPConnection(); // Create IP connection
$ai = new BrickletAnalogInV3(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$ai->setDebouncePeriod(10000);

// Register voltage callback to function cb_voltage
$ai->registerCallback(BrickletAnalogInV3::CALLBACK_VOLTAGE, 'cb_voltage');

// Configure threshold for voltage "outside of 5 to 0 V" (unit is mV)
// with a debounce period of 1s (1000ms)
$ai->setVoltageCallbackConfiguration(1000, FALSE, 'o', 5*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
