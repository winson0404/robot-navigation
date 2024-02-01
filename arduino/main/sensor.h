#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H
#include <Arduino.h>
#include "utils.h"

namespace sensor
{
    const int trigPin = 10;  
    const int echoPin = 3;
    const int right_IR_Pin = 11;
    const int mid_IR_Pin = A0;
    const int left_IR_Pin = 9;

    void sensor_setup();
    uint8_t sensor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p);
    void get_sensor_data(bool &comm_state, uint8_t &task_state, comms::packet &p);
    float get_ultra_sonic_data();

}

#endif