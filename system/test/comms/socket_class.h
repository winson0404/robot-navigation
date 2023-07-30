#pragma once
#include <unistd.h>
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
    class SocketServer
    {
    private:
        const int port;
        SOCKET server_socket;
        const char *host;

    public:
        SocketServer(const int port, const char *host);
        ~SocketServer();
        bool Listen();
        SOCKET GetServerSocket();
        RemoteClient AcceptConnection();
        bool Send(SOCKET socket, const char *data, int data_length);
        bool Receive(SOCKET socket, char *data, int data_length);
        void CloseSocket(SOCKET socket);
    };

    class SocketClient
    {
    private:
        const int port;
        SOCKET client_socket;
        const char *host;

    public:
        SocketClient(const int port, const char *host);
        ~SocketClient();
        int Connect(const char *host);
        bool Send(SOCKET socket, const char *data, int data_length);
        bool Receive(SOCKET socket, char *data, int data_length);
        void CloseSocket(SOCKET socket);
    };
}