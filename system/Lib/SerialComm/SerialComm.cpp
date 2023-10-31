#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "SerialComm.h"

namespace comms
{
    // SerialComm::SerialComm(){}

    SerialComm::SerialComm(comms::SerialSettings _settings):
        settings(_settings)
    {
        // this->settings = _settings;
        // this->serial_port_name = serial_port_name;
        // LibSerial::SerialPort serial_port;
        this->Initialize();
    }

    // SerialComm::SerialComm(){
    //     this->settings = comms::SerialSettings();
    //     this->serial_port_name = this->settings.serial_port_name;
    //     this->Initialize();
    // }

    SerialComm::~SerialComm()
    {
        this->serial_port.Close();
    }

    bool SerialComm::Initialize()
    {
        try
        {

            this->serial_port.Open(this->settings.serial_port_name);

            // Set the serial port settings (make sure to match with Arduino's settings)
            this->serial_port.SetBaudRate(this->settings.baud_rate);
            this->serial_port.SetFlowControl(this->settings.flow_control);
            this->serial_port.SetParity(this->settings.parity);
            this->serial_port.SetStopBits(this->settings.stop_bits);
            std::cout << "Successfully connected to " << this->serial_port_name << std::endl;

        }

        catch (const LibSerial::OpenFailed &)
        {
            std::cerr << "The serial port did not open correctly." << std::endl;
            exit(EXIT_FAILURE);
        }

        return true;


    }

    bool SerialComm::Send(std::string data){
        this->serial_port.Write(data);
        this->serial_port.DrainWriteBuffer();

        return true;
    }
    
    bool SerialComm::Receive(std::string &data){
        this->serial_port.Read(data);


        return true;
    }

}