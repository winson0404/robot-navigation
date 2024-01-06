#ifndef _UART_H
#define _UART_H

#include <Arduino.h>
#include <SoftwareSerial.h>

namespace serial{

    void recvWithStartEndMarkers(SoftwareSerial &ser, char *data, bool &newData, int startMarker, char endMarker);

}

#endif