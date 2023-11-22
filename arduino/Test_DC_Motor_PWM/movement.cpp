#include "movement.h"

void init_setup(){
  // Initialize output pins.
  pinMode(pMotorL1, OUTPUT);
  pinMode(pMotorL2, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorLSpeedCtrl, OUTPUT);
  pinMode(pMotorRSpeedCtrl, OUTPUT);

  // Set motor direction, CW.
  // spinFront(LEFT_MOTOR);
  spinStop(LEFT_MOTOR);
  spinStop(RIGHT_MOTOR);

  nSpeed = 0;

}

void adjustSpeed(int motor, int speed){

  if (motor == LEFT_MOTOR)
    analogWrite(pMotorLSpeedCtrl,speed);
  if (motor == RIGHT_MOTOR)
    analogWrite(pMotorRSpeedCtrl,speed);
}

void moveFront(){
  spinFront(LEFT_MOTOR);
  spinFront(RIGHT_MOTOR);
}

void moveBack(){
  spinBack(LEFT_MOTOR);
  spinBack(RIGHT_MOTOR);
}

void moveLeft(){
  spinStop(LEFT_MOTOR);
  spinFront(RIGHT_MOTOR);
}

void moveRight(){
  spinStop(RIGHT_MOTOR);
  spinFront(LEFT_MOTOR);
}

void moveStop(){
  spinStop(LEFT_MOTOR);
  spinStop(RIGHT_MOTOR);
}

void spinFront(int motor){
  if (motor == LEFT_MOTOR){
    digitalWrite(pMotorL1, HIGH);
    digitalWrite(pMotorL2, LOW);
  }

  if (motor == RIGHT_MOTOR){
    digitalWrite(pMotorR1, HIGH);
    digitalWrite(pMotorR2, LOW); 
  }
}

void spinBack(int motor){
  if (motor == LEFT_MOTOR){
    digitalWrite(pMotorL1, LOW);
    digitalWrite(pMotorL2, HIGH);
  }

  if (motor == RIGHT_MOTOR){
    digitalWrite(pMotorR1, LOW);
    digitalWrite(pMotorR2, HIGH); 
  }            

}

void spinStop(int motor){
  if (motor == LEFT_MOTOR){
    digitalWrite(pMotorL1, LOW);
    digitalWrite(pMotorL2, LOW);
  }

  if (motor == RIGHT_MOTOR){
    digitalWrite(pMotorR1, LOW);
    digitalWrite(pMotorR2, LOW); 
  }        
}
