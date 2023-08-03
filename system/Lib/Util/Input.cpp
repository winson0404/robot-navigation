#include <iostream>

#include "Utils.h"

namespace inputs{
    VisualOutput *visual_process(char *buffer){
        std::cout << buffer << std::endl;
    }
    Sensor1Output *sensor1_process(char *buffer){
        std::cout << buffer << std::endl;
    }
}