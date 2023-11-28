#include "movement.h"


namespace movement
{

  void movement_setup()
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
    spinStop(LEFT_MOTOR);
    spinStop(RIGHT_MOTOR);

    nSpeed = 0;
  }

  void movement_handler(char *data, char &packet_length, char &task)
  {
    char direction_size = *data;
    data++;
    char direction = *data;
    data++;
    char speed_size = *data;
    data++;
    unsigned short speed = convert_ushort(data, (int)speed_size);

    // char* movement;
    // Serial.readBytes(movement,1);

    switch ((int)direction)
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

    analogWrite(pMotorLSpeedCtrl, (int)speed);
    analogWrite(pMotorRSpeedCtrl, (int)speed);
    task = comms::COMMS;
    packet_length = 1;
    *data = 1;
  // }
  // moveFront();
  // delay(1000);
  // moveStop();
  // delay(2000);
  // moveBack();
  // delay(1000);
  // moveStop();
  // delay(2000);
  // delay(250);
  /*
  nSpeed = nSpeed + 10;
  analogWrite(pMotorLSpeedCtrl,nSpeed);
  analogWrite(pMotorRSpeedCtrl,nSpeed);
  delay(250);        // Delay 400 msec.
  if (nSpeed > 250)  // Max value for nSpeed is 255.
  {
    nSpeed = 0;
  }
 */

  // Set motor A and B directions, CW, speed 150.

  // spinFront(LEFT_MOTOR);
  // analogWrite(pMotorLSpeedCtrl,200);
  // spinFront(RIGHT_MOTOR);
  // analogWrite(pMotorRSpeedCtrl,200);

  // delay(2000);
  // Set motor A and B directions, CW, speed 70.
  // spinStop(LEFT_MOTOR);
  // delay(2000);
  // spinBack(LEFT_MOTOR);

  // digitalWrite(pMotorL1, HIGH);
  // digitalWrite(pMotorL2, LOW);
  // analogWrite(pMotorLSpeedCtrl,50);
  // digitalWrite(pMotorR1, HIGH);
  // digitalWrite(pMotorR2, LOW);
  // analogWrite(pMotorRSpeedCtrl,50);
  // delay(2000);
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
