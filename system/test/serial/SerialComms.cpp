#include "SerialComms.h"
#include <iostream>
#include <linux/serial.h>

namespace serial
{
    SerialPort::SerialPort()
    {
        portOpen = false;
    }

    SerialPort::SerialPort(const char *portName)
    {
        // open port and check if successful or else throw error in C style
        // library
        portOpen = false;
    }

    SerialPort::~SerialPort()
    {
        // close port
        closePort();
    }

    int SerialPort::openPort(const char *portName)
    {
        // open port and check if successful or else throw error with C
        serialPort = open(portName, O_RDWR | O_SYNC);
        if (serialPort < 0)
        {
            printf("Error %i from open: %s\n", errno, strerror(errno));
            return 1;
        }
        else
        {
            portOpen = true;
        }

        if (tcgetattr(serialPort, &tty) != 0)
        {
            printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
            return 1;
        }

        tty.c_cflag &= ~CSTOPB;                                                      // 1 stop bit
        tty.c_cflag &= ~CRTSCTS;                                                     // no hardware flow control
        tty.c_cflag |= CREAD | CLOCAL;                                               // turn on READ & ignore ctrl lines
        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);                              // raw mode
        tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // no software flow control
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
        tty.c_oflag &= ~OPOST;                                                       // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR;                                                       // Prevent conversion of newline to carriage return/line feed
        tty.c_cflag &= ~CSIZE;                                                       // Clear all the size bits, then use one of the statements below
        tty.c_cflag |= CS8;                                                          // 8 bits per byte (most common)

        tty.c_cc[VMIN] = 0;   // Read at least 5 characters
        tty.c_cc[VTIME] = 0; // Wait for up to 1 second (10 deciseconds)
        if (cfsetospeed(&tty, B19200) != 0 || cfsetispeed(&tty, B19200) != 0)
        {
            std::cerr << "Error setting baud rate" << std::endl;
            close(serialPort);
            return 1;
        }

        // Save tty settings, also checking for error
        if (tcsetattr(serialPort, TCSANOW, &tty) != 0)
        {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }

        return 0;
    }

    void SerialPort::closePort()
    {
        // close port
        close(serialPort);
        portOpen = false;
    }

    void SerialPort::setBaudRate(int baudRate)
    {
        // set baud rate
        cfsetospeed(&tty, baudRate);
        cfsetispeed(&tty, baudRate);

        // Save tty settings, also checking for error
        if (tcsetattr(serialPort, TCSANOW, &tty) != 0)
        {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }

    void SerialPort::setCharacterSize(int characterSize)
    {
        // set character size
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= characterSize;

        // Save tty settings, also checking for error
        if (tcsetattr(serialPort, TCSANOW, &tty) != 0)
        {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }

    void SerialPort::setFlowControl(int flowControl)
    {
        // set flow control
        tty.c_cflag &= ~CRTSCTS; // no hardware flow control
        if (flowControl == 1)
        {
            tty.c_cflag |= CRTSCTS;
        }
        else if (flowControl == 2)
        {
            tty.c_cflag |= IXON | IXOFF | IXANY;
        }

        // Save tty settings, also checking for error
        if (tcsetattr(serialPort, TCSANOW, &tty) != 0)
        {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }

    void SerialPort::setParity(int parity)
    {
        // set parity
        tty.c_cflag &= ~PARENB;
        if (parity == 1)
        {
            tty.c_cflag |= PARENB;
        }
        else if (parity == 2)
        {
            tty.c_cflag |= PARODD;
        }

        // Save tty settings, also checking for error
        if (tcsetattr(serialPort, TCSANOW, &tty) != 0)
        {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }

    void SerialPort::setStopBits(int stopBits)
    {
        // set stop bits
        tty.c_cflag &= ~CSTOPB;
        if (stopBits == 2)
        {
            tty.c_cflag |= CSTOPB;
        }

        // Save tty settings, also checking for error
        if (tcsetattr(serialPort, TCSANOW, &tty) != 0)
        {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }

    void SerialPort::writeByte(char byte)
    {
        if (portOpen)
        {

            // write byte
            write(serialPort, &byte, 1);
        }
    }

    void SerialPort::writeBytes(char *bytes, int length)
    {
        if (portOpen)
        {

            // write bytes
            write(serialPort, bytes, length);
        }
    }

    void SerialPort::readByte(char *byte)
    {
        if (portOpen)
        {

            // read bytes
            int n = read(serialPort, byte, 1);
            if (n < 0)
            {
                printf("Error reading: %s\n", strerror(errno));
            }
            if (n == 0)
            {
                printf("Read nothing!\n");
            }
            printf("Read %i bytes\n", n);
        }
        else{
            std::cout << "Port not open" << std::endl;
        }
    }

    void SerialPort::readBytes(char *bytes, int length)
    {
        if (portOpen)
        {

            // read bytes
            read(serialPort, bytes, length);
        }
    }

    void SerialPort::flushIOBuffers()
    {
        if (portOpen)
        {

            // flush io buffers
            tcflush(serialPort, TCIOFLUSH);
        }
    }

    void SerialPort::drainWriteBuffer()
    {
        if (portOpen)
        {

            // drain write buffer
            tcdrain(serialPort);
        }
    }

    void SerialPort::setSerialPortBlockingStatus(bool blocking)
    {
        if (portOpen)
        {

            // set blocking status
            if (blocking)
            {
                fcntl(serialPort, F_SETFL, 0);
            }
            else
            {
                fcntl(serialPort, F_SETFL, FNDELAY);
            }
        }
    }

    int SerialPort::isDataAvailable()
    {
        if (portOpen)
        {

            // check if data is available
            int bytesAvailable;
            if (ioctl(serialPort, FIONREAD, &bytesAvailable) == -1)
            {
                printf("Error %i from ioctl: %s\n", errno, strerror(errno));
                return -1;
            }

            if (bytesAvailable > 0)
            {
                return bytesAvailable;
            }
            else
            {
                return 0;
            }
        }
        else
        {

            return -2;
        }
    }

    int SerialPort::getDTR()
    {
        if (portOpen)
        {

            // get DTR status
            int status;
            ioctl(serialPort, TIOCMGET, &status);
            if (status & TIOCM_DTR)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    int SerialPort::getRTS()
    {
        if (portOpen)
        {

            // get RTS status
            int status;
            ioctl(serialPort, TIOCMGET, &status);
            if (status & TIOCM_RTS)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

}