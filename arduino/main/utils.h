#ifndef UTILS_FUNCTIONS_H
#define UTILS_FUNCTIONS_H

#define d_int int16_t

#include <Arduino.h>
#include <string.h>

namespace constants
{
    constexpr uint8_t MAX_NUM_DATA = 32;
    constexpr uint8_t RECEIVE_BUFFER_SIZE = 64;

    // serial setting
    const int BAUD_RATE = 9600;
    const uint8_t START_MARKER = 0x3c; //'<'
    const uint8_t END_MARKER = 0x3e;   //'>'

    // tx, rx pins
    const int TX_PIN = 12;
    const int RX_PIN = 13;

    // _UART
    const uint8_t ACKNOWLEDGE_SUCCESS = 0;
    const uint8_t ACKNOWLEDGE_FAIL = 1;

    // serial mode
    const bool RECEIVE = 1;
    const bool SEND = 0;

    // comms_tasks
    const uint8_t COMMS = 1;
    const uint8_t COMMS_TASK1 = 10;

    // sensor_tasks
    const uint8_t SENSOR_MODE = 2;
    const uint8_t SENSOR_DATA_REQ = 20;
    // const uint8_t SENSOR_IR_REQ = 21;

    // motor tasks
    const uint8_t MOTOR = 3;
    const uint8_t MOTOR_MOVE = 30;                // computer ask the robot to move
    const uint8_t MOTOR_TURN = 31;                // computer ask the robot to move
    const uint8_t MOTOR_VELOCITY_REQ = 32;    // computer request speed of velocity
    // const uint8_t MOTOR_VELOCITY_SEND = 31;    // send computer speed of velocity

}

namespace comms
{

    /*
    TODO: he pointer to data_length and data is gonna cost issue in the future,
    this code shouldn't be used for thread in the future, need to modify to std::vector to
    ensure pass on functions are deep copy, or create your own copy function
    */
    struct packet
    {
        uint8_t checksum;                             // 1 byte
        uint8_t packet_length;                        // 1 byte
        uint8_t task;                                 // 1 byte
        uint8_t num_data;                             // 1 byte
        uint8_t data_length[constants::MAX_NUM_DATA]; // 1 byte
        d_int data[constants::MAX_NUM_DATA];       // 2 bytes
    };

    packet construct_packet_from_data_buffer(uint8_t *data);
    packet construct_packet(uint8_t task, uint8_t num_data, uint8_t *data_length, d_int *data);


}

unsigned short convert_ushort(char *data, char data_length);

#endif