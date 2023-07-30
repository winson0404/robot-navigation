#include <unistd.h>
#include <iostream>

#include "socket_class.h"


int main(){

    comms::SocketClient client(PORT, HOST);

    const char* data = "Hi from Client ";
    char output[1024];
    // connect to server
    int host_socket= client.Connect(HOST);
    
    // send data to server


    client.Send(host_socket, data, 1024);

    client.Receive(host_socket, output, 1024);
    std::cout << "Received from server: " << output << std::endl;

}