#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <functional>
#include <cstring>

#include "socket_class.h"

comms::SocketServer::SocketServer(const int port, const char *host) : port(port), server_socket(INVALID_SOCKET), host(host)
{
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == -1)
    {
        std::cerr << "Error creating socket!" << std::endl;
        exit(1);
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(this->port);
    server_address.sin_addr.s_addr = this->host ? inet_addr(this->host) : INADDR_ANY;

    if (bind(this->server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind the server socket." << std::endl;

        exit(1);
    }
}

comms::SocketServer::~SocketServer()
{
    this->CloseSocket(server_socket);
}

SOCKET comms::SocketServer::GetServerSocket()
{
    return this->server_socket;
}

bool comms::SocketServer::Listen()
{

    // TODO: might need to check if using max will affect memory
    if (listen(this->server_socket, 5) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen on the server socket." << std::endl;
        return false;
    }

    std::cout << "Server listening on port " << this->port << "..." << std::endl;
    return true;
}
// Recursive variadic template function to handle multiple arguments
// template <typename ReturnType, typename InputType, typename... Args>
// ReturnType performOperation(InputType arg, Args... args, std::function<ReturnType(InputType, Args...)> operation) {
//     return operation(arg, args...);
// }

comms::RemoteClient comms::SocketServer::AcceptConnection()
{
    sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);
    RemoteClient client;
    SOCKET clientSocket = accept(server_socket, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to accept client connection." << std::endl;
    }
    else
    {
        std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
    }

    client.client_socket = clientSocket;
    client.sin_addr = inet_ntoa(clientAddr.sin_addr);
    client.sin_port = ntohs(clientAddr.sin_port);

    return client;
}

bool comms::SocketServer::Send(SOCKET clientSocket, const char *data, int data_length)
{
    std::cout << "Sending " << data << " to client\n"
              << std::endl;
    int bytesSent = send(clientSocket, data, data_length, 0);
    return bytesSent == data_length;
}

bool comms::SocketServer::Receive(SOCKET clientSocket, char *data, int data_length)
{
    char buffer[data_length] = {0};
    int bytesRead = recv(clientSocket, buffer, data_length, 0);
    if (bytesRead <= 0)
    {
        std::cout << "Socket closed\n";
        CloseSocket(clientSocket);
        return false;
    }
    data = strcpy(data, buffer);

    return true;
}

void comms::SocketServer::CloseSocket(SOCKET socket)
{
    close(socket);
}

//============== CLIENT ==============

comms::SocketClient::SocketClient(const int port, const char *host) : port(port), client_socket(INVALID_SOCKET), host(host)
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        std::cerr << "Error creating socket!" << std::endl;
        exit(1);
    }
}

comms::SocketClient::~SocketClient()
{
    this->CloseSocket(client_socket);
}

int comms::SocketClient::Connect(const char *host)
{
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    // server_address.sin_addr.s_addr = host ? inet_addr(host) : INADDR_ANY;
    inet_pton(AF_INET, host, &(server_address.sin_addr));
    if (connect(this->client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to server." << std::endl;
        return false;
    }

    std::cout << "Connected to server." << std::endl;
    return client_socket;
}


bool comms::SocketClient::Send(SOCKET clientSocket, const char *data, int data_length)
{
    int bytesSent = send(clientSocket, data, data_length, 0);
    return bytesSent == data_length;
}

bool comms::SocketClient::Receive(SOCKET clientSocket, char *data, int data_length)
{
    char buffer[data_length] = {0};
    int bytesRead = recv(clientSocket, buffer, data_length, 0);
    if (bytesRead <= 0)
    {
        this->CloseSocket(clientSocket);
        return false;
    }

    data = strcpy(data, buffer);

    return true;
}

void comms::SocketClient::CloseSocket(SOCKET socket)
{
    close(socket);
}