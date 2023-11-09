#pragma once
#include <unistd.h>
#include <libserial/SerialPort.h>
#include <string>
// #include <iostream>
namespace comms
{
    struct SerialSettings{
        SerialSettings():
            // serial_port_name("/dev/ttyACM0"),
            flow_control(LibSerial::FlowControl::FLOW_CONTROL_NONE),
            parity(LibSerial::Parity::PARITY_NONE),
            stop_bits(LibSerial::StopBits::STOP_BITS_1),
            baud_rate(LibSerial::BaudRate::BAUD_9600)
        {}
        char *serial_port_name;
        LibSerial::FlowControl flow_control;
        LibSerial::Parity parity;
        LibSerial::StopBits stop_bits;
        LibSerial::BaudRate baud_rate;
    };

    class SerialComm
    {
    private:
        comms::SerialSettings settings;
        char *serial_port_name;
        LibSerial::SerialPort serial_port;

    public:
        // SerialComm();
        SerialComm(comms::SerialSettings settings);
        ~SerialComm();
        bool Initialize();
        bool Send(std::string data);
        bool Receive(std::string &data);
        bool ClosePort();
    };
}