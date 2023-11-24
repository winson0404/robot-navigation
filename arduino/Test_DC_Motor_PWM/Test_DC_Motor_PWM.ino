#include "movement.h"
#include "comms.h"

void setup()
{
  movement_setup();
  comms_setup(9600);
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  char* data;
  comms_listener(listener_mode, &task, data, &packet_length);
  switch((int)task){
  case(SENSOR):
    //do something with sensor handler
    break;
  case(MOVEMENT):
    movement_handler(data, &packet_length, &task);
    listener_mode = SEND;
    break;
  default:
    break;

  }
}
