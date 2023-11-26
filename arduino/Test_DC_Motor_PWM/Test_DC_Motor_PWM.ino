#include "movement.h"
#include "sensor_handler.h"
#include "comms.h"
#include "utils.h"

public
enum TaskState
{
  COMM = 1,
  MOVEMENT = 2,
  SENSOR = 3
};

TaskState task_state = COMM;
char comms_state = comms::RECEIVE;
char packet_length = 0;

void setup()
{
  movement_setup();
  comms_setup(9600);
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  char *data;
  if (tast == COMM)
  {
    comms::comms_listener(comms_state, &task, data, &packet_length);
    switch ((int)task)
    {
    case (comms::SENSOR):
      task_state = SENSOR;
      break;
    case (comms::MOVEMENT):
      task_state = MOVEMENT;
      break;
    }
    else if (task == MOVEMENT)
    {
      movement::movement_handler(data, &packet_length, &task);
      comms_state = comms::SEND;
      task_state = COMM;
    }
    else if (task == SENSOR)
    {
      sensor::sensor_handler(data, &packet_length, &task);
      comms_state = comms::SEND;
      task_state = COMM;
    }
  }
}
