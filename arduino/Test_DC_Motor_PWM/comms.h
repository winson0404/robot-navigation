#ifndef COMMS_FUNCTIONS_H
#define COMMS_FUNCTIONS_H
#include <Arduino.h>

namespace comms
{
    const bool RECEIVE = 1;
    const bool SEND = 0;

    // tasks
    const bool COMMS_REPORT = 1;
    const bool SENSOR = 2;
    const bool MOTOR = 3;

    bool listener_mode;
    char task;
    char packet_length;

    int sum(float a, float b);
    int dif(float a, float b);

    void comms_setup(int baud_rate);

    void comms_listener(bool comm_state, char &task, char *data, char &packet_length);

    bool handle_receive(char &task, char *data, char &packet_length);
    bool handle_send(char &task, char *data, char &packet_length);

}
