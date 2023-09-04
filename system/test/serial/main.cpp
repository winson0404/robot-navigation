#include <iostream>
#include <libserial/SerialPort.h>

int main()
{
    int move = 1;

    LibSerial::SerialPort serial_port;
    // send
    std::cout << "================Sending Control: " << move << std::endl;

    // try
    // {
        // Open the Serial Port at the desired hardware port.
        serial_port.Open("/dev/ttyACM0");
    // }
    // catch (const LibSerial::OpenFailed &)
    // {
    //     std::cerr << "The serial port did not open correctly." << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    // Set the serial port settings (make sure to match with Arduino's settings)
    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_9600);

    // Set the number of data bits.
    // serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_5);

    // Turn off hardware flow control.
    serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);

    // Disable parity.
    serial_port.SetParity(LibSerial::Parity::PARITY_NONE);

    // Set the number of stop bits.
    serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

    serial_port.Write("abc");

    // Wait until the data has actually been transmitted.
    serial_port.DrainWriteBuffer();
}