// Control of DC Motor Driver with speed adjustment using PWM
// Author  : Fabian Kung
// Date    : 4 Dec 2015

// Pins declarations
// Control pins for the DC Motor Driver

#include "movement.h"



void setup()
{
  init_setup();
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  char* movement;
  Serial.readBytes(movement,1);

  // switch((int)movement){
  //   case MOVEMENT_FORWARD:
  //     moveFront();break;
  //   case MOVEMENT_LEFT:
  //     moveLeft();break;
  //   case MOVEMENT_RIGHT:
  //     moveRight();break;
  //   default:
  //     break;
  // }

  moveFront();
  delay(2000);
  moveStop();
  delay(2000);
  moveBack();
  delay(2000);
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
