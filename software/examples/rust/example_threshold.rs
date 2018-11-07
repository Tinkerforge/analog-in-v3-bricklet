use std::{error::Error, io, thread};
use tinkerforge::{analog_in_v3_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Analog In Bricklet 3.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let ai = AnalogInV3Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Create receiver for voltage events.
    let voltage_receiver = ai.get_voltage_receiver();

    // Spawn thread to handle received events. This thread ends when the `ai` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for voltage in voltage_receiver {
            println!("Voltage: {} V", voltage as f32 / 1000.0);
        }
    });

    // Configure threshold for voltage "smaller than 5 V"
    // with a debounce period of 1s (1000ms).
    ai.set_voltage_callback_configuration(1000, false, '<', 5 * 1000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
