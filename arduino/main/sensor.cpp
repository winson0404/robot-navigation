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
    case constants::SENSOR_ULTRASONIC_REQ:
      get_ultrasonic_data(comm_state, task_state, p);
      break;
    case constants::SENSOR_IR_REQ:
      //do sth
      break;

    default:
      break;
    }
  }

  void get_ultrasonic_data(bool &comm_state, uint8_t &task_state, comms::packet &p){

    /*
    Controls the movement of the robot based on the data received from the comms module.


    data protocol:
    Sequence    Content
     1         ultrasonic data (float*100 => uint16_t)
    */



    float sensor_data = 312.41;

    uint16_t output = sensor_data*100;
    
    uint8_t *data_length = (uint8_t *)malloc(1);
    uint16_t *data = (uint16_t *)malloc(1);

    data_length[0] = 2;

    data[0] = output;

    p = comms::construct_packet(p.task, 1, data_length, data);
    Serial.println("In sensor");
    task_state = constants::COMMS;
    comm_state = constants::SEND;

    free(data_length);
    free(data);
      
    // return output;

  }
}