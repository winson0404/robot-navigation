#ifndef MOVEMENT_FUNCTIONS_H
#define MOVEMENT_FUNCTIONS_H
#include <Arduino.h>

namespace sensor
{
    void sensor_setup();
    void sensor_handler(char *data, char &packet_length, char &task);

}

#endif