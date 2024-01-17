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
    case constants::MOTOR_MOVE:
      move_motor_task(comm_state, task_state, p);
      break;

    case constants::MOTOR_TURN:
      turn_motor_task(comm_state, task_state, p);
      break;

    default:
      break;
    }
  }

  void move_motor_task(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {
    /*
    Controls the movement of the robot based on the data received from the comms module.


    data protocol:
    Sequence    Content
     1         speed (0-255) (+32767)
     2         duration (0-255)
    */

    d_int speed = p.data[0];
    d_int duration = p.data[1];

    Serial.print("speed: ");

    move_motor_with_speed(speed);

    // switch (direction)
    // {
    // case MOVEMENT_FORWARD:
    //   moveFront();
    //   break;
    // case MOVEMENT_LEFT:
    //   moveLeft();
    //   break;
    // case MOVEMENT_RIGHT:
    //   moveRight();
    //   break;
    // default:
    //   moveStop();
    // }

    // adjustSpeed(LEFT_MOTOR, speed);
    // adjustSpeed(RIGHT_MOTOR, speed);

    delay(duration);

    moveStop();
    task_state = constants::COMMS;

  }

  void turn_motor_task(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {
    /*
    Controls the movement of the robot based on the data received from the comms module.

    data protocol:
    Sequence    Content
     1         rotation angle (0-255) (+32767)
    */
    float rotation_angle = p.data[0] / 100;

    Serial.print("rotation_angle: ");

    Serial.println(rotation_angle);

    if (rotation_angle > 0)
    {
      moveRight();
    }
    else if (rotation_angle < 0)
    {
      moveLeft();
    }
    else
    {
      moveStop();
    }

    delay(500);
    moveStop();

    task_state = constants::COMMS;
  }



  void move_motor_with_speed(int velocity)
  {

    //make sure velocity is in range -255 to 255
    if (velocity > 255)
      velocity = 255;
    else if (velocity < -255)
      velocity = -255;

    if (velocity > 0)
    {
      moveFront(); //place holder cause motor dont accept speed to move wheel
      adjustSpeed(LEFT_MOTOR, velocity);
      adjustSpeed(RIGHT_MOTOR, velocity);
    }
    else if (velocity < 0)
    {
      moveBack(); //place holder cause motor dont accept speed to move wheel

      velocity = -velocity;
      adjustSpeed(LEFT_MOTOR, velocity);
      adjustSpeed(RIGHT_MOTOR, velocity);
    }
    else
    {
      moveStop(); //place holder cause motor dont accept speed to move wheel
      adjustSpeed(LEFT_MOTOR, velocity);
      adjustSpeed(RIGHT_MOTOR, velocity);
    }
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
    spinBack(LEFT_MOTOR);
    spinFront(RIGHT_MOTOR);
  }

  void moveRight()
  {
    spinBack(RIGHT_MOTOR);
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
