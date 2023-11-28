#include <iostream>
#include <string>
#include <cstring>
#include <libserial/SerialPort.h>

int main()
{
    int move = 1;

    LibSerial::SerialPort serial_port;
    serial_port.Open("/dev/ttyACM0");
    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_9600);

    // Turn off hardware flow control.
    serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);

    // Disable parity.
    serial_port.SetParity(LibSerial::Parity::PARITY_NONE);

    bool send_task = false;
    int i = 0;  
    while (true)
    {
        // std::string data = "test";
        char data = 'a';
        serial_port.WriteByte(data);

        // receive
        std::string received_data;
        // serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        // serial_port.DrainWriteBuffer();
        std::cout<< "Waiting for data..." << std::endl;
        // serial_port.SetSerialPortBlockingStatus(false);
        std::cout << "DTR: " << serial_port.GetDTR() << std::endl;
        std::cout << "RTS: " << serial_port.GetRTS() << std::endl;
        // serial_port.Read(received_data, 1);

        std::cout<< "Received Data: " << received_data << std::endl;
        
    }

}