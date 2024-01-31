#include "motor.h"
#include "sensor.h"
#include "comms.h"
#include "utils.h"

comms::packet p;
uint8_t task_state = constants::COMMS;
bool comms_state = constants::RECEIVE;
SoftwareSerial ser(constants::RX_PIN, constants::TX_PIN);

void setup()
{
  Serial.begin(9600);
  p.task = constants::COMMS;
  motor::motor_setup();
  sensor::sensor_setup();
  comms::comms_setup(ser);
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  if (task_state == constants::COMMS)
  {
    comms::comms_listener(ser, comms_state, task_state, p);
  }
  else if (task_state == constants::MOTOR)
  {
    // Serial.println(p.task);
    motor::motor_handler(comms_state, task_state, p);
  }
  else if (task_state == constants::SENSOR_MODE)
  {
    sensor::sensor_handler(comms_state, task_state, p);
  }
  else{
    task_state = constants::COMMS;
    comms_state = constants::RECEIVE;
  }
}
