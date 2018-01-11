Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Analog In Bricklet 3.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ai As New BrickletAnalogInV3(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current voltage (unit is mV)
        Dim voltage As Integer = ai.GetVoltage()
        Console.WriteLine("Voltage: " + (voltage/1000.0).ToString() + " V")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
