#include "TcpSocket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace comms
{
    SocketServer::SocketServer(const int port, const char *host) : Socket(port, host)
    {
    }

    SocketServer::~SocketServer()
    {
        this->CloseSocket(_socket);
    }

    bool SocketServer::Listen(int max)
    {

        // TODO: might need to check if using max will affect memory
        if (listen(this->_socket, max) == SOCKET_ERROR)
        {
            std::cerr << "Failed to listen on the server socket." << std::endl;
            return false;
        }

        std::cout << "Server listening on port " << this->port << "..." << std::endl;
        return true;
    }

    RemoteClient SocketServer::AcceptConnection()
    {
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        RemoteClient client;

        client.client_socket = accept(this->_socket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (client.client_socket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept the client socket." << std::endl;
            exit(1);
        }

        client.sin_addr = inet_ntoa(clientAddr.sin_addr);
        client.sin_port = ntohs(clientAddr.sin_port);

        std::cout << "Client connected on port " << client.sin_port << std::endl;

        return client;
    }
}