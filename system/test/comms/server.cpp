#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <vector>

#include "socket_class.h"

void handleClient(comms::RemoteClient client, comms::SocketServer &server)
{
    char buffer[1024];
    while (true)
    {

        // Receive data from the client

        if (!server.Receive(client.client_socket, buffer, 1024))
        {
            // exit and terminate the thread
            server.CloseSocket(client.client_socket);
            break;
        }
        std::cout << "Received from client: " << buffer << std::endl;

        server.Send(client.client_socket, buffer, strlen(buffer));

        // int bytesRead = recv(client.client_socket, buffer, sizeof(buffer), 0);
        // if (bytesRead <= 0) {
        //     break; // Client disconnected or error occurred
        // }

        // // Process the data (here, we'll just print it)
        // buffer[bytesRead] = '\0';
        // std::cout << "Received from client: " << buffer << std::endl;

        // // Echo back the received data to the client
        // send(client.client_socket, buffer, bytesRead, 0);
    }
}

int main()
{
    const int BACKLOG = 5;
    comms::RemoteClient client;
    comms::SocketServer server(PORT, HOST);

    // std::cout << "Server started. Listening on port " << comms:PORT << std::endl;

    std::vector<std::thread> clientThreads;

    // Accept incoming connections and create a thread for each client
    while (true)
    {

        if (!server.Listen())
        {
            std::cerr << "Error listening for connections!" << std::endl;
            return 1;
        }
        client = server.AcceptConnection();
        if (client.client_socket == -1)
        {
            std::cerr << "Error accepting connection!" << std::endl;
            continue;
        }
        std::cout << "Connection accepted from " << client.sin_addr << ":" << client.sin_port << std::endl;
        clientThreads.emplace_back(handleClient, client, std::ref(server));
        clientThreads.back().detach();
    }

    server.CloseSocket(server.GetServerSocket());

    return 0;
}