#include "sensor_handler.h"

void sensor::sensor_setup()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
}

void sensor::sensor_handler(char *data, char &packet_length, char &task)
{
    unsigned short sensor_data = 3152;
    char sensor_data_size = 2;
    unsigned short *sensor_data_ptr = &sensor_data;
    char* data_chunk;
    //put size of sensor data, and then sensor data
    // memcpy(data_chunk, &sensor_data_size, 1);
    data_chunk = (char*)malloc(sensor_data_size+1);
    data_chunk[0] = sensor_data_size;
    
    memcpy(data_chunk+1, sensor_data_ptr, sensor_data_size);
    data = data_chunk;


    packet_length = 3;
    task = comms::SENSOR;
}