#include <iostream>
#include <unistd.h>   
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>

#include "Lib/Socket/TcpSocket.h"
#include "Lib/Util/Utils.h"


int main(){

    inputs::InputState *input_state = new inputs::InputState();

    //run listener and control in different threads
    std::thread listener_thread(system_fnc::listener, input_state);
    std::thread control_thread(system_fnc::status_checker, input_state);

    listener_thread.join();
    control_thread.join();

    return 0;
}
