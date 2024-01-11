#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H
#include <Arduino.h>
#include "utils.h"

namespace sensor
{
    void sensor_setup();
    void sensor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p);
    void get_ultrasonic_data(bool &comm_state, uint8_t &task_state, comms::packet &p);
}

#endif