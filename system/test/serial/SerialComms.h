#pragma once

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/ioctl.h>

namespace serial{
    class SerialPort{
        public:
            SerialPort();
            SerialPort(const char* portName);
            ~SerialPort();
            int openPort(const char* portName);
            void closePort();
            void setBaudRate(int baudRate);
            void setCharacterSize(int characterSize);
            void setFlowControl(int flowControl);
            void setParity(int parity);
            void setStopBits(int stopBits);
            void writeByte(char byte);
            void writeBytes(char* bytes, int length);
            void readByte(char* byte);
            void readBytes(char* bytes, int length);
            void flushIOBuffers();
            void drainWriteBuffer();
            void setSerialPortBlockingStatus(bool blocking);
            int isDataAvailable();
            int getDTR();
            int getRTS();
        private:
            int serialPort;
            struct termios tty;
            struct termios tty_old;
            bool portOpen;
    };


}