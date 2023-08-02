#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "TcpSocket.h"

namespace comms
{

    SocketClient::SocketClient(const int port, const char *host) : Socket(port, host)
    {
    }

    SocketClient::~SocketClient()
    {
        this->CloseSocket(_socket);
    }

    int SocketClient::Connect(const char *host)
    {
        sockaddr_in server_address{};
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        // server_address.sin_addr.s_addr = host ? inet_addr(host) : INADDR_ANY;
        inet_pton(AF_INET, host, &(server_address.sin_addr));
        if (connect(this->_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
        {
            std::cerr << "Failed to connect to server." << std::endl;
            return false;
        }

        std::cout << "Connected to server." << std::endl;
        return this->_socket;
    }

}