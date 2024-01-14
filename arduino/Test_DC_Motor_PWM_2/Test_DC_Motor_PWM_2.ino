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
// Global variables                           
static int nSpeed;

void setup()
{
  pinMode(pMotorL1, OUTPUT);
  pinMode(pMotorL2, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorLSpeedCtrl, OUTPUT);
  pinMode(pMotorRSpeedCtrl, OUTPUT);

  // Set motor direction, CW.

  nSpeed = 0;
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
  // analogWrite(pMotorLSpeedCtrl,200);
  // spinFront(RIGHT_MOTOR);
  // analogWrite(pMotorRSpeedCtrl,200);  

  // delay(2000);   
  // Set motor A and B directions, CW, speed 70.
  // spinStop(LEFT_MOTOR);
  // delay(2000);
  // spinBack(LEFT_MOTOR);
  analogWrite(pMotorLSpeedCtrl,50);
  analogWrite(pMotorRSpeedCtrl,50);  

  digitalWrite(pMotorL1, HIGH);
  digitalWrite(pMotorL2, LOW);
  digitalWrite(pMotorR1, HIGH);
  digitalWrite(pMotorR2, LOW);
  delay(2000);
}
