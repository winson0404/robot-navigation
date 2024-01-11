#include "sensor.h"
namespace sensor
{
  void sensor_setup()
  {
    int b = 1;
  }

  void sensor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {    
    switch (p.task)
    {
    case constants::SENSOR_DATA_REQ:
      get_sensor_data(comm_state, task_state, p);
      break;
    default:
      break;
    }
  }

  void get_sensor_data(bool &comm_state, uint8_t &task_state, comms::packet &p){

    /*
    Controls the movement of the robot based on the data received from the comms module.


    data protocol:
    Sequence    Content
     1         ultrasonic data (float*100 => d_int)
     1         infared ray data (float*100 => d_int)
    */



    float sensor_data_1 = 312.41;
    d_int output_1 = sensor_data_1*100;


    float sensor_data_2 = -134.67;
    d_int output_2 = sensor_data_2*100;

    uint8_t num_data = 2;
    
    uint8_t *data_length = (uint8_t *)malloc(num_data);
    d_int *data = (d_int *)malloc(num_data);

    data_length[0] = sizeof(output_1);
    data[0] = output_1;

    data_length[1] = sizeof(output_2);
    data[1] = output_2;

    p = comms::construct_packet(p.task, num_data, data_length, data);
    Serial.println("In sensor");
    task_state = constants::COMMS;
    comm_state = constants::SEND;

    free(data_length);
    free(data);
      
    // return output;

  }
}