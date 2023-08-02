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

    class Socket
    {
    protected:
        const int port;
        SOCKET _socket;
        const char *host;

    public:
        Socket(const int port, const char *host);
        ~Socket();
        SOCKET GetSocket();
        bool Send(SOCKET clientSocket, const char *data, int data_length);
        bool Receive(SOCKET clientSocket, char *data, int data_length);
        void CloseSocket(SOCKET socket);
    };

    class SocketServer : public Socket
    {
    public:
        SocketServer(const int port, const char *host);
        ~SocketServer();
        bool Listen(int max);
        RemoteClient AcceptConnection();
    };

    class SocketClient : public Socket
    {

    public:
        SocketClient(const int port, const char *host);
        ~SocketClient();
        int Connect(const char *host);
    };
}