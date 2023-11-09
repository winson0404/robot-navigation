#include <iostream>
#include <unistd.h>   
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>

#include "Lib/Socket/TcpSocket.h"
#include "Lib/SerialComm/SerialComm.h"
#include "Lib/Util/Utils.h"

// void HighLevelAlgorithm(){

// }

int main(){

    inputs::InputState *input_state = new inputs::InputState();
    comms::SerialSettings settings = comms::SerialSettings();

    comms::SerialComm serial_comm(settings);
    settings.serial_port_name = "/dev/ttyACM0";
    settings.baud_rate = LibSerial::BaudRate::BAUD_9600;

    //run listener and control in different threads
    std::thread serial_listener_thread(system_fnc::serial_listener, serial_comm, input_state);
    std::thread tcp_listener_thread(system_fnc::tcp_listener, input_state);
    // listener_thread.detach();
    // std::thread control_thread(system_fnc::status_checker, input_state);


    system_fnc::status_checker(input_state);

    serial_listener_thread.join();
    tcp_listener_thread.join();
    // control_thread.join();

    return 0;
}
