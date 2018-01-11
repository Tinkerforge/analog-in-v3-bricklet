var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Analog In Bricklet 3.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var ai = new Tinkerforge.BrickletAnalogInV3(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        ai.setDebouncePeriod(10000);

        // Configure threshold for voltage "outside of 5 to 0 V" (unit is mV)
        // with a debounce period of 1s (1000ms)
        ai.setVoltageCallbackConfiguration(1000, false, 'o', 5*1000, 0);
    }
);

// Register voltage callback
ai.on(Tinkerforge.BrickletAnalogInV3.CALLBACK_VOLTAGE,
    // Callback function for voltage callback (parameter has unit mV)
    function (voltage) {
        console.log('Voltage: ' + voltage/1000.0 + ' V');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
