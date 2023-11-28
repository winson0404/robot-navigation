#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H
#include <Arduino.h>

#include "comms.h"

namespace sensor
{
    void sensor_setup();
    void sensor_handler(char *data, char &packet_length, char &task);

}

#endif