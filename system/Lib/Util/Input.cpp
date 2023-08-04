#include <iostream>
#include <map>
#include "Utils.h"

namespace inputs
{
    VisualOutput *visual_process(char *buffer)
    {
        VisualOutput *output = new VisualOutput();
        int a = 0;
        for (int i = 0; i < 3; i++)
        {
            if (i == 2)
                a = (int)buffer[i];
            std::cout << (int)buffer[i] << std::endl;
        }
        switch(a){
            case 0:
                std::cout << "forward" << std::endl;
                break;
            case 1:
                std::cout << "left" << std::endl;
                break;
            case 2:
                std::cout << "right" << std::endl;
                break;
            case 3:
                std::cout << "backward" << std::endl;
                break;
            default:
                std::cout << "default" << std::endl;
                break;
        }
        return output;
    }
    Sensor1Output *sensor1_process(char *buffer)
    {
        std::cout << buffer << std::endl;
        Sensor1Output *output = new Sensor1Output();
        return output;
    }
}