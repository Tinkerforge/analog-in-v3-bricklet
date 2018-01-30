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
        // Configure threshold for voltage "smaller than 5 V"
        // with a debounce period of 1s (1000ms)
        ai.setVoltageCallbackConfiguration(1000, false, '<', 5*1000, 0);
    }
);

// Register voltage callback
ai.on(Tinkerforge.BrickletAnalogInV3.CALLBACK_VOLTAGE,
    // Callback function for voltage callback
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
