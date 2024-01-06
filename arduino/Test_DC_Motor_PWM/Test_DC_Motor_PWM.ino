#include "movement.h"
#include "comms.h"
#include "utils.h"

char task_state = comms::COMMS;
bool comms_state = comms::RECEIVE;
char packet_length = 0;

void setup()
{
  movement::movement_setup();
  sensor::sensor_setup();
  comms::comms_setup();
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  comms::packet p;
  if (task_state == comms::COMMS)
  {
    comms::comms_listener(comms_state, p);
  }
  else if (task_state == comms::MOTOR)
  {
    movement::movement_handler(p);
    comms_state = comms::SEND;
    task_state = comms::COMMS;
  }
  else if (task_state == comms::SENSOR)
  {
    sensor::sensor_handler(p);
    comms_state = comms::SEND;
    task_state = comms::COMMS;
  }
}
