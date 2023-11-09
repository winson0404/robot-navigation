#include <cstring>
#include <iostream>
#include <vector>
#include <thread>

#include "Utils.h"

namespace system_fnc
{
    void handleClient(comms::RemoteClient client, comms::SocketServer &server, inputs::InputState *input_state)
    {
        while (true)
        {
            std::cout << "start handleClient" << std::endl;
            if (input_state == nullptr)
            {

                std::cerr << "Input state is null!" << std::endl;
                exit(1);
            }
            char tag[1];
            std::cout << "Before receive" << std::endl;
            server.Receive(client.client_socket, tag, 1);
            int received_tag = (int)tag[0];
            std::cout << "tag: " << received_tag << std::endl;
            // tag[0] = tag[0] - '0';

            char length[1];
            server.Receive(client.client_socket, length, 1);
            int data_length = (int)length[0];
            std::cout << "length: " << data_length << std::endl;

            // char *buffer = new char[data_length];
            char buffer[data_length];
            server.Receive(client.client_socket, buffer, data_length);
            switch (received_tag)
            {
            case inputs::VISUAL_TAG:
                input_state->visual = inputs::visual_process(buffer, data_length);
                break;
            case inputs::SENSOR1_TAG:
                input_state->sensor1 = inputs::sensor1_process(buffer, data_length);
                break;
            default:
                std::cerr << "Invalid tag received: " << tag << std::endl;
                exit(1);
            }
        }
        // delete[] buffer;
    }

    void tcp_listener(inputs::InputState *input_state)
    {
        const int port = 12345;
        const char *host = "127.0.0.1";
        // comms::SocketServer server(12345, "localhost");

        const int BACKLOG = 5;
        comms::RemoteClient client;
        comms::SocketServer server(port, host);

        // std::cout << "Server started. Listening on port " << comms:PORT << std::endl;

        std::vector<std::thread> clientThreads;

        // Accept incoming connections and create a thread for each client
        while (true)
        {

            if (!server.Listen(BACKLOG))
            {
                std::cerr << "Error listening for connections!" << std::endl;
                exit(1);
            }
            client = server.AcceptConnection();
            if (client.client_socket == -1)
            {
                std::cerr << "Error accepting connection!" << std::endl;
                continue;
            }
            std::cout << "Connection accepted from " << client.sin_addr << ":" << client.sin_port << std::endl;
            clientThreads.emplace_back(system_fnc::handleClient, client, std::ref(server), input_state);
            clientThreads.back().detach();
        }

        server.CloseSocket(server.GetSocket());
    }
}