#ifndef _UART_H
#define _UART_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "utils.h"

namespace serial{

    void send_acknowledge(SoftwareSerial &ser, uint8_t task, uint8_t start_marker, uint8_t end_marker);
    bool receive_acknowledge(SoftwareSerial &ser, uint8_t task,uint8_t start_marker, uint8_t end_marker);
    void recv_with_start_end_markers(SoftwareSerial &ser, uint8_t data_buffer[], bool &newData, uint8_t startMarker, uint8_t endMarker);
    void send_with_start_end_markers(SoftwareSerial &ser, comms::packet p, uint8_t startMarker, uint8_t endMarker);
    void clear_buffer(SoftwareSerial &ser);
}

#endif