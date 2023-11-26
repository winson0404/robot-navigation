#include "comms.h"

namespace comms
{
  void comms_setup(int baud_rate)
  {
    Serial.being(baud_rate);
    task = -1;
    packet_length = 0;
    listener_mode = RECEIVE;
  }

  void comms_listener(bool comm_state, char &task, char *data, char &packet_length)
  {
    if (comm_state == RECEIVE)
    {
      handle_receive(&task, data, &packet_length);
    }
    else
    {
      handle_send(&task, data, &packet_length);
    }
  }

  bool handle_receive(char &task, char *data, char &packet_length)
  {
    char start_bit = -1;
    // char task= 0, packet_length=0;
    Serial.readBytes(data, 1);
    if ((int)*data == 0)
    { // start bit received
      data = nullptr;
      Serial.readBytes(data, 1);
      task = *data;
      data = nullptr;
      Serial.readBytes(data, 1);
      packet_length = *data;
      data = nullptr;
      Serial.readBytes(data, (int)packet_length);
      // Serial.print("Task:");
      // Serial.println((int) task);
      // Serial.print("Packet Length:");
      // Serial.println((int) packet_length);
    }
  }

  bool handle_send(char &task, char *data, char &packet_length)
  {
    char start_bit = 0;
    Serial.write(&start_bit, 1);
    Serial.write(&task, 1);
    Serial.write(&packet_length, 1);
    Serial.write(data, packet_length);
  }
}
