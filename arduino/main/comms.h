#ifndef COMMS_FUNCTIONS_H
#define COMMS_FUNCTIONS_H
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "utils.h"
#include "_uart.h"

namespace comms
{
    // Initialize SoftwareSerial

    int sum(float a, float b);
    int dif(float a, float b);

    void comms_setup(SoftwareSerial &ser);

    void comms_listener(SoftwareSerial &ser, bool &comm_state, uint8_t &task_state, packet &p);

    bool handle_receive(SoftwareSerial &ser, packet &p, uint8_t data[]);
    bool handle_send(SoftwareSerial &ser, packet &p);

}

#endif
