#pragma once
#include <sys/socket.h>
#include <string>
#include "utils.h"
#include <thread>
#include <iostream>

//create a class for tcp socket

namespace inputs{


    void* receive_handler(char* data){
        // char buffer[1024] = {0};
        // int valread = ::receive( socket , buffer, 1);
        // std::cout << buffer << std::endl;
        int tag = data[0];
        switch(tag){
            case INPUT_TAG::VISUAL_TAG: // visual inputs
                inputs::visual_process(data+1);
                break;
            case INPUT_TAG::SENSOR1_TAG: // sensor1 inputs
                inputs::sensor1_process(data+1);
                break;
            default:
                std::cerr<<"Invalid input tag received, "<< tag <<std::endl;
                break;
        }
    }
}