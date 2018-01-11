function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletAnalogInV3;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Analog In Bricklet 3.0

    ipcon = IPConnection(); % Create IP connection
    ai = handle(BrickletAnalogInV3(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register voltage callback to function cb_voltage
    set(ai, 'VoltageCallback', @(h, e) cb_voltage(e));

    % Set period for voltage callback to 1s (1000ms) without a threshold
    ai.setVoltageCallbackConfiguration(1000, false, 'x', 0, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for voltage callback (parameter has unit mV)
function cb_voltage(e)
    fprintf('Voltage: %g V\n', e.voltage/1000.0);
end
