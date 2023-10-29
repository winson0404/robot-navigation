#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "SerialComm.h"

namespace comms
{

    SerialComm::SerialComm(const char* serial_port_name)
    {
        this->serial_port_name = serial_port_name;
        LibSerial::SerialPort serial_port;
    }

    SerialComm::~SerialComm()
    {
    }

    bool SerialComm::Initialize(){
        try{
            
            this->serial_port.Open(this->serial_port_name);

        }

         catch (const LibSerial::OpenFailed &)
        {
            std::cerr << "The serial port did not open correctly." << std::endl;
            exit(EXIT_FAILURE);
        }

    }


    
}