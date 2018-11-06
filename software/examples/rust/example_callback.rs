use std::{error::Error, io, thread};
use tinkerforge::{analog_in_v3_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Analog In Bricklet 3.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let analog_in_v3_bricklet = AnalogInV3Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for voltage events.
    let voltage_listener = analog_in_v3_bricklet.get_voltage_receiver();
    // Spawn thread to handle received events. This thread ends when the analog_in_v3_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in voltage_listener {
            println!("Voltage: {}{}", event as f32 / 1000.0, " V");
        }
    });

    // Set period for voltage callback to 1s (1000ms) without a threshold
    analog_in_v3_bricklet.set_voltage_callback_configuration(1000, false, 'x', 0, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
