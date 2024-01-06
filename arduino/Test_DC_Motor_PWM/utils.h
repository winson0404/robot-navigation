#ifndef UTILS_FUNCTIONS_H
#define UTILS_FUNCTIONS_H
#include <Arduino.h>
#include <string.h>

namespace constants{
    constexpr uint8_t MAX_NUM_DATA = 32;
    constexpr uint8_t RECEIVE_BUFFER_SIZE = 64;
}



unsigned short convert_ushort(char* data, char data_length);


#endif