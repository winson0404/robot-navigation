#include "motor.h"

namespace motor
{

  void motor_setup()
  {
    // Initialize output pins.
    pinMode(pMotorL1, OUTPUT);
    pinMode(pMotorL2, OUTPUT);
    pinMode(pMotorR1, OUTPUT);
    pinMode(pMotorR1, OUTPUT);
    pinMode(pMotorLSpeedCtrl, OUTPUT);
    pinMode(pMotorRSpeedCtrl, OUTPUT);

    // Set motor direction, CW.a
    // spinFront(LEFT_MOTOR);
    // moveFront();
    spinStop(LEFT_MOTOR);
    spinStop(RIGHT_MOTOR);
  }

  void motor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {

    switch (p.task)
    {
    case constants::MOTOR_VELOCITY_REQUEST:
      // do sth
      break;
    case constants::MOTOR_MOVE:
      move_motor(comm_state, task_state, p);
      break;

    default:
      break;
    }
  }

  void move_motor(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {
    /*
    Controls the movement of the robot based on the data received from the comms module.


    data protocol:
    Sequence    Content
     1         direction (0: forward, 1: left, 2: right, 3: back)
     2         speed (0-255)
     3         duration (0-255)
    */

    uint16_t direction = p.data[0];
    uint16_t speed = p.data[1];
    uint16_t duration = p.data[2];


    switch (direction)
    {
    case MOVEMENT_FORWARD:
      moveFront();
      break;
    case MOVEMENT_LEFT:
      moveLeft();
      break;
    case MOVEMENT_RIGHT:
      moveRight();
      break;
    default:
      moveStop();
    }

    adjustSpeed(LEFT_MOTOR, speed);
    adjustSpeed(RIGHT_MOTOR, speed);

    delay(duration);

    moveStop();
    task_state = constants::COMMS;

  }

  void adjustSpeed(int motor, int speed)
  {

    if (motor == LEFT_MOTOR)
      analogWrite(pMotorLSpeedCtrl, speed);
    if (motor == RIGHT_MOTOR)
      analogWrite(pMotorRSpeedCtrl, speed);
  }

  void moveFront()
  {     
     Serial.println("moveFront");
      // Serial.print("Task: ");
      // Serial.println(direction);

    spinFront(LEFT_MOTOR);
    spinFront(RIGHT_MOTOR);
  }

  void moveBack()
  {
    spinBack(LEFT_MOTOR);
    spinBack(RIGHT_MOTOR);
  }

  void moveLeft()
  {
    spinStop(LEFT_MOTOR);
    spinFront(RIGHT_MOTOR);
  }

  void moveRight()
  {
    spinStop(RIGHT_MOTOR);
    spinFront(LEFT_MOTOR);
  }

  void moveStop()
  {
    spinStop(LEFT_MOTOR);
    spinStop(RIGHT_MOTOR);
  }

  void spinFront(int motor)
  {
    if (motor == LEFT_MOTOR)
    {
      digitalWrite(pMotorL1, HIGH);
      digitalWrite(pMotorL2, LOW);
    }

    if (motor == RIGHT_MOTOR)
    {
      digitalWrite(pMotorR1, HIGH);
      digitalWrite(pMotorR2, LOW);
    }
  }

  void spinBack(int motor)
  {
    if (motor == LEFT_MOTOR)
    {
      digitalWrite(pMotorL1, LOW);
      digitalWrite(pMotorL2, HIGH);
    }

    if (motor == RIGHT_MOTOR)
    {
      digitalWrite(pMotorR1, LOW);
      digitalWrite(pMotorR2, HIGH);
    }
  }

  void spinStop(int motor)
  {
    if (motor == LEFT_MOTOR)
    {
      digitalWrite(pMotorL1, LOW);
      digitalWrite(pMotorL2, LOW);
    }

    if (motor == RIGHT_MOTOR)
    {
      digitalWrite(pMotorR1, LOW);
      digitalWrite(pMotorR2, LOW);
    }
  }
}
