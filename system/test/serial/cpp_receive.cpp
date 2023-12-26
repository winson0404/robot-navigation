#include <iostream>
#include <string>
// #include <libserial/SerialPort.h>
// #include "SerialComms.h"

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()
#include <sys/ioctl.h>

int isDataAvailable(int serial_port)
{
    // check if data is available
    int bytesAvailable;
    if (ioctl(serial_port, FIONREAD, &bytesAvailable) == -1)
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

void read_data(char start_marker, char end_marker, char *data, int serial_port, int num_bytes)
{
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    char rc;
    memset(data, 0, 32);

    int i = 0;
    while (num_bytes > 0)
    {
        std::cout << "iteration: " << i++ << std::endl;
        // serial_port->readByte(&rc);
        read(serial_port, &rc, 1);
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
        num_bytes--;
        std::cout << "Available data after read: " << num_bytes << std::endl;

        // deallocate rc
        // free(rc);
    }
}

int main()
{
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open("/dev/ttyACM0", O_RDWR);

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;         // Clear all bits that set the data size
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B19200);
    cfsetospeed(&tty, B19200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    char* message = "<abcde>";

    // Write to serial port
    write(serial_port, message, strlen(message));

    std::cout<< "Message sent" << message << std::endl;

    // Allocate memory for read buffer, set size according to your needs
    char read_buf [32];
    memset(&read_buf, '\0', sizeof(read_buf));


    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    while(true){
        int available_data = isDataAvailable(serial_port);
        if (available_data > 5){
            std::cout << "Data available: "<< available_data << std::endl;
            read_data('<', '>', read_buf, serial_port, available_data);
            printf("Read: %s\n", read_buf);
            if (strcmp(read_buf, "defah") == 0){
                printf("Matched\n");
                break;
            }
        }
        
    }
    // read_data('<', '>', read_buf, serial_port);
    // printf("Read: %s\n", read_buf);

    // close(serial_port);


    return 0;

}