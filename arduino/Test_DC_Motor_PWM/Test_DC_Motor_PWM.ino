#include "movement.h"
#include "sensor_handler.h"
#include "comms.h"
#include "utils.h"

char task_state = comms::COMMS;
bool comms_state = comms::RECEIVE;
char packet_length = 0;

void setup()
{
  movement::movement_setup();
  comms::comms_setup(9600);
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  char *data;
  if (task_state == comms::COMMS)
  {
    comms::comms_listener(comms_state, task_state, data, packet_length);
  }
  else if (task_state == comms::MOTOR)
  {
    movement::movement_handler(data, packet_length, task_state);
    comms_state = comms::SEND;
    task_state = comms::COMMS;
  }
  else if (task_state == comms::SENSOR)
  {
    sensor::sensor_handler(data, packet_length, task_state);
    comms_state = comms::SEND;
    task_state = comms::COMMS;
  }
}
