#include <unistd.h>
#include <iostream>

#include "socket_class.h"


int main(){

    comms::SocketClient client(PORT, HOST);

    const char* data = "From Client 1";
    char output[1024];
    // connect to server
    int client_socket= client.Connect(HOST);
    
    // send data to server


    client.Send(client_socket, data, 1024);

    client.Receive(client_socket, output, 1024);

}