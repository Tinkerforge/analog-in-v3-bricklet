function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Analog In Bricklet 3.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    ai = javaObject("com.tinkerforge.BrickletAnalogInV3", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    ai.setDebouncePeriod(10000);

    % Register voltage callback to function cb_voltage
    ai.addVoltageCallback(@cb_voltage);

    % Configure threshold for voltage "outside of 5 to 0 V" (unit is mV)
    % with a debounce period of 1s (1000ms)
    ai.setVoltageCallbackConfiguration(1000, false, "o", 5*1000, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for voltage callback (parameter has unit mV)
function cb_voltage(e)
    fprintf("Voltage: %g V\n", e.voltage/1000.0);
end
