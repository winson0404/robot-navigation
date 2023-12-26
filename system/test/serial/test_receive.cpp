#include <iostream>
#include <string>
#include <cstring>
#include <libserial/SerialPort.h>
#include "SerialComms.h"

void read_data(char start_marker, char end_marker, char *data, serial::SerialPort *serial_port)
{
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    char rc;
    memset(data, 0, 32);

    int i = 0;
    while (serial_port->isDataAvailable() > 0)
    {
        std::cout << "iteration: " << i++ << std::endl;
        serial_port->readByte(&rc);
        std::cout << "Received byte: " << rc << std::endl;
        if (recvInProgress == true)
        {
            if (rc != end_marker)
            {
                data[ndx] = rc;
                ndx++;
                if (ndx >= 32)
                {
                    ndx = 32 - 1;
                }
            }
            else
            {
                std::cout << "Received last byte: " << rc << std::endl;
                data[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                // empty input buffer
                // while (serial_port->isDataAvailable() > 0)
                // {
                //     serial_port->readByte(rc);
                // }
                break;
            }
        }
        else if (rc == start_marker)
        {
            recvInProgress = true;
        }

        std::cout << "Available data after read: " << serial_port->isDataAvailable() << std::endl;

        // deallocate rc
        // free(rc);
    }
}

int main()
{
    // initialize serial_port

    static serial::SerialPort* serial_port = new serial::SerialPort();
    if (serial_port->openPort("/dev/ttyACM0") != 0)
    {
        std::cout << "Error opening port" << std::endl;
        return 1;
    }
    // serial_port.setBaudRate(B9600);
    // serial_port.setCharacterSize(CS8);
    // serial_port.setFlowControl(0);
    // serial_port.setParity(0);
    // serial_port.setStopBits(1);

    char *data = (char *)malloc(32 * sizeof(char));
    data = "<abcde>";
    serial_port->writeBytes(data, 8);
    std::cout << "Data sent: " << data << std::endl;
    // serial_port.flushIOBuffers();

    char *received_data = (char *)malloc(32 * sizeof(char));
    while (true)
    {
        // std::cout << serial_port.isDataAvailable() << std::endl;
        // std::cout << "Waiting for data..." << std::endl;

        // if data available is 0, sleep for 1 second
        // if (serial_port->isDataAvailable() == 0){
            // std::cout << "No data available" << std::endl;
            // sleep(1);
        // }

        if (serial_port->isDataAvailable() == -1)
            {
                std::cout << "Error reading data" << std::endl;
                return 1;
            }

        // if (serial_port->isDataAvailable() == 0)
        // {
        //     std::cout << "No data available" << std::endl;
        // }

        if (serial_port->isDataAvailable() >= 5)
        {


            std::cout << "Data Available: " << serial_port->isDataAvailable() << std::endl;
            read_data('<', '>', received_data, serial_port);
            std::cout << "Received Data: " << received_data << std::endl;
            if (received_data == "defgh")
            {
                std::cout << "========================================" << std::endl;
                break;
            }
        }
    }
}