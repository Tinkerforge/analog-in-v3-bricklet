function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletAnalogInV3;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Analog In Bricklet 3.0

    ipcon = IPConnection(); % Create IP connection
    ai = handle(BrickletAnalogInV3(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current voltage (unit is mV)
    voltage = ai.getVoltage();
    fprintf('Voltage: %g V\n', voltage/1000.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
