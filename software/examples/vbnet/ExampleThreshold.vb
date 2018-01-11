Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Analog In Bricklet 3.0

    ' Callback subroutine for voltage callback (parameter has unit mV)
    Sub VoltageCB(ByVal sender As BrickletAnalogInV3, ByVal voltage As Integer)
        Console.WriteLine("Voltage: " + (voltage/1000.0).ToString() + " V")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ai As New BrickletAnalogInV3(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        ai.SetDebouncePeriod(10000)

        ' Register voltage callback to subroutine VoltageCB
        AddHandler ai.VoltageCallback, AddressOf VoltageCB

        ' Configure threshold for voltage "outside of 5 to 0 V" (unit is mV)
        ' with a debounce period of 1s (1000ms)
        ai.SetVoltageCallbackConfiguration(1000, False, "o"C, 5*1000, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
