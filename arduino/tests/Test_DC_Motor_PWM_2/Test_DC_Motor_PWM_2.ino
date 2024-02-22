// Control of DC Motor Driver with speed adjustment using PWM
// Author  : Fabian Kung
// Date    : 4 Dec 2015

// Pins declarations
// Control pins for the DC Motor Driver

const int LEFT_MOTOR = 0;
const int RIGHT_MOTOR = 1;
const int MOVEMENT_FORWARD = 0;
const int MOVEMENT_RIGHT = 1;
const int MOVEMENT_LEFT = 2;

const int pMotorL1 = 2;
const int pMotorL2 = 4;
const int pMotorR1 = 7;
const int pMotorR2 = 8;
const int pMotorLSpeedCtrl = 5;  // These two pins has PWM capability.  Also
const int pMotorRSpeedCtrl = 6;  // on the Arduino reference, pin 5 and 6 has
                           // similar PWM clock.
void setup()
{
  pinMode(pMotorL1, OUTPUT);
  pinMode(pMotorL2, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorLSpeedCtrl, OUTPUT);
  pinMode(pMotorRSpeedCtrl, OUTPUT);
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  // nSpeed = nSpeed + 10;
  // analogWrite(pMotorLSpeedCtrl,nSpeed);
  // analogWrite(pMotorRSpeedCtrl,nSpeed);
  // delay(250);        // Delay 400 msec.
  // if (nSpeed > 250)  // Max value for nSpeed is 255.
  // {
    // nSpeed = 0;    
  // }
  
  // Set motor A and B directions, CW, speed 150.
  
  // spinFront(LEFT_MOTOR);
  analogWrite(pMotorLSpeedCtrl,200);
  // spinFront(RIGHT_MOTOR);
  analogWrite(pMotorRSpeedCtrl,200);  

  // delay(2000);   
  // Set motor A and B directions, CW, speed 70.
  // spinStop(LEFT_MOTOR);
  // delay(2000);
  // spinBack(LEFT_MOTOR);

  
  // // turn front
  // digitalWrite(pMotorL1, HIGH);
  // digitalWrite(pMotorL2, LOW);

  // // turn front
  // digitalWrite(pMotorR1, HIGH);
  // digitalWrite(pMotorR2, LOW);

  // analogWrite(pMotorLSpeedCtrl,50);
  // analogWrite(pMotorRSpeedCtrl,50);  

  
  // delay(2000);

  // analogWrite(pMotorLSpeedCtrl,100);
  // analogWrite(pMotorRSpeedCtrl,100);  

  // right_motor_rotate back
  digitalWrite(pMotorR1, LOW);
  digitalWrite(pMotorR2, HIGH);

  // left_motor_rotate front
  digitalWrite(pMotorL1, HIGH);
  digitalWrite(pMotorL2, LOW);


  // analogWrite(pMotorLSpeedCtrl,100);
  // analogWrite(pMotorRSpeedCtrl,100);  

  // delay(1700);
  // delay(880);

  
  // // right_motor_rotate back
  // digitalWrite(pMotorR1, LOW);
  // digitalWrite(pMotorR2, LOW);

  // // left_motor_rotate front
  // digitalWrite(pMotorL1, LOW);
  // digitalWrite(pMotorL2, LOW);

  // delay(1000);
}
