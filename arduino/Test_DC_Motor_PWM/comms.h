#ifndef COMMS_FUNCTIONS_H
#define COMMS_FUNCTIONS_H
#include <Arduino.h>
#include <SoftwareSerial.h>

namespace comms
{
    struct packet
    {
        uint8_t checksum;            //1 byte
        uint8_t packet_length;       //1 byte   
        uint8_t task;                //1 byte
        uint8_t num_data;            //1 byte
        uint8_t *data_length;        //1 byte
        uint16_t *data;              //2 bytes
    };

    // serial setting
    const int BAUD_RATE = 9600;

    // tx, rx pins
    const int TX_PIN = 12;
    const int RX_PIN = 13;

    // serial mode
    const bool RECEIVE = 1;
    const bool SEND = 0;

    // tasks
    const uint8_t COMMS = 1;
    const uint8_t SENSOR = 2;
    const uint8_t MOTOR = 3;

    // Initialize SoftwareSerial
    SoftwareSerial ser(RX_PIN, TX_PIN);

    int sum(float a, float b);
    int dif(float a, float b);

    void comms_setup();

    void comms_listener(bool &comm_state, packet &p);

    bool handle_receive(packet &p);
    bool handle_send(packet &p);
    void packet_post_process(packet &p, uint8_t *data);
    void free_packet(packet &p);

}

#endif
