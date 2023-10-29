#pragma once
#include <unistd.h>
#include <libserial/SerialPort.h>
// #include <iostream>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 12345
#define HOST "127.0.0.1"

namespace comms
{
    struct RemoteClient
    {
        int client_socket;
        char *sin_addr;
        __uint16_t sin_port;
    };

    class SerialComm
    {
    private:
        const char *serial_port_name;
        LibSerial::SerialPort serial_port;
        SOCKET _socket;
        const char *host;

    public:
        SerialComm(const char *serial_port);
        ~SerialComm();
        bool Initialize();
        bool Send();
        bool Receive();
        bool ClosePort();
    };
}