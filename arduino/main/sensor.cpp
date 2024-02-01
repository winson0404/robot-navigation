#include "sensor.h"
namespace sensor
{
  void sensor_setup()
  {	
    pinMode(trigPin, OUTPUT);
	  pinMode(echoPin, INPUT);
    pinMode(right_IR_Pin, INPUT);
    pinMode(left_IR_Pin, INPUT);
    pinMode(mid_IR_Pin, INPUT); 

  }

  uint8_t sensor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {    
    switch (p.task)
    {
    case constants::SENSOR_DATA_REQ:
      get_sensor_data(comm_state, task_state, p);
      return constants::TASK_SUCCESS;
    default:
      return constants::TASK_FAIL;
    }
  }

  void get_sensor_data(bool &comm_state, uint8_t &task_state, comms::packet &p){
    /*
    Controls the movement of the robot based on the data received from the comms module.
            Byte_Size     Sequence                 Content
                2            1         ultrasonic data (float*100 => int16_t)
                2            2         front infared ray data (float*100 => int16_t)
                2            3         left infared ray data (float*100 => int16_t)
                2            4         right infared ray data (float*100 => int16_t)
    */

    // float front_us = get_ultra_sonic_data();
    float front_us = get_ultra_sonic_data();
    d_int output_1 = front_us*100;
    // Serial.print("Ultrasonic data: ");
    // Serial.println(front_us);


    bool irValue_right = digitalRead(right_IR_Pin);
    bool irValue_left = digitalRead(left_IR_Pin);
    bool irValue_mid = digitalRead(mid_IR_Pin);
    
    // float front_ir = -134.67;
    d_int output_2 = (int)(irValue_mid == LOW);

    // float left_ir = -31.67;
    d_int output_3 = (int)(irValue_left == LOW);

    // float right_ir = 123.67;
    d_int output_4 = (int)(irValue_right == LOW);

    if (irValue_right == LOW)
    {
        Serial.println("Right 1"); // 1 means detecting something
    }
    else
    {
        Serial.println("Right 0"); // 0 means not detecting anything
    }

    if (irValue_left == LOW)
    {
        Serial.println("Left 1");
    }
    else
    {
        Serial.println("Left 0");
    }

    if (irValue_mid == LOW)
    {
        Serial.println("Mid 1");
    }
    else
    {
        Serial.println("Mid 0");
    }

    uint8_t num_data = 4;
    
    uint8_t *data_length = (uint8_t *)malloc(num_data);
    d_int *data = (d_int *)malloc(num_data);

    data_length[0] = sizeof(output_1);
    data[0] = output_1;

    data_length[1] = sizeof(output_2);
    data[1] = output_2;

    data_length[2] = sizeof(output_3);
    data[2] = output_3;

    data_length[3] = sizeof(output_4);
    data[3] = output_4;

    p = comms::construct_packet(p.task, num_data, data_length, data);
    // Serial.println("In sensor");
    task_state = constants::COMMS;
    comm_state = constants::SEND;

    free(data_length);
    free(data);
      
    // return output;

  }

  float get_ultra_sonic_data(){
    float duration, distance;  

    digitalWrite(trigPin, LOW);  
    delayMicroseconds(2);  
    digitalWrite(trigPin, HIGH);  
    delayMicroseconds(10);  
    digitalWrite(trigPin, LOW);  

    duration = pulseIn(echoPin, HIGH);

    //distance in cm
    distance = (duration*.0343)/2;  


    return distance;

  }
}