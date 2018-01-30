using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Analog In Bricklet 3.0

	// Callback function for voltage callback
	static void VoltageCB(BrickletAnalogInV3 sender, int voltage)
	{
		Console.WriteLine("Voltage: " + voltage/1000.0 + " V");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletAnalogInV3 ai = new BrickletAnalogInV3(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register voltage callback to function VoltageCB
		ai.VoltageCallback += VoltageCB;

		// Set period for voltage callback to 1s (1000ms) without a threshold
		ai.SetVoltageCallbackConfiguration(1000, false, 'x', 0, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
