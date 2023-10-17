#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "SerialComm.h"

namespace comms
{

    SerialComm::SerialComm(const int port, const char *host) : port(port), _socket(INVALID_SOCKET), host(host)
    {
        this->_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (this->_socket == -1)
        {
            std::cerr << "Error creating socket!" << std::endl;
            exit(1);
        }

        sockaddr_in _address{};
        _address.sin_family = AF_INET;
        _address.sin_port = htons(this->port);
        _address.sin_addr.s_addr = this->host ? inet_addr(this->host) : INADDR_ANY;

        if (bind(this->_socket, (struct sockaddr *)&_address, sizeof(_address)) == SOCKET_ERROR)
        {
            std::cerr << "Failed to bind the server socket." << std::endl;

            exit(1);
        }
    }

    SerialComm::~SerialComm()
    {
        this->CloseSocket(_socket);
    }

    SOCKET SerialComm::GetSocket()
    {
        return this->_socket;
    }


    bool SerialComm::Send(SOCKET clientSocket, const char *data, int data_length)
    {
        int bytesSent = send(clientSocket, data, data_length, 0);
        return bytesSent == data_length;
    }

    bool SerialComm::Receive(SOCKET clientSocket, char *data, int data_length)
    {
        char buffer[data_length] = {0};
        int bytesRead = recv(clientSocket, buffer, data_length, 0);
        // if (bytesRead <= 0)
        // {
        //     std::cout << "Socket closed\n";
        //     CloseSocket(clientSocket);
        //     return false;
        // }
        data = strcpy(data, buffer);

        return true;
    }

    void SerialComm::CloseSocket(SOCKET socket)
    {
        close(socket);
    }

}