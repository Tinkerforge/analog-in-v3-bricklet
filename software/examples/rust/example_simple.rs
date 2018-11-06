use std::{error::Error, io};

use tinkerforge::{analog_in_v3_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Analog In Bricklet 3.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let analog_in_v3_bricklet = AnalogInV3Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current voltage
    let voltage = analog_in_v3_bricklet.get_voltage().recv()?;
    println!("Voltage: {}{}", voltage as f32 / 1000.0, " V");

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
