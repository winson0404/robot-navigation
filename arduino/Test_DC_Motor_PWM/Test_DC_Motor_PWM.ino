// Control of DC Motor Driver with speed adjustment using PWM
// Author  : Fabian Kung
// Date    : 4 Dec 2015

// Pins declarations
// Control pins for the DC Motor Driver
int pMotorL1 = 2;
int pMotorL2 = 4;
int pMotorR1 = 7;
int pMotorR2 = 8;
int pMotorLSpeedCtrl = 5;  // These two pins has PWM capability.  Also
int pMotorRSpeedCtrl = 6;  // on the Arduino reference, pin 5 and 6 has
                           // similar PWM clock.
// Global variables                           
int nSpeed;                        

void setup()
{
  // Initialize output pins.
  pinMode(pMotorL1, OUTPUT);
  pinMode(pMotorL2, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorR1, OUTPUT);
  pinMode(pMotorLSpeedCtrl, OUTPUT);
  pinMode(pMotorRSpeedCtrl, OUTPUT);
  
  // Set motor direction, CW.
  digitalWrite(pMotorL1, HIGH);
  digitalWrite(pMotorL2, LOW);
 
  digitalWrite(pMotorR1, HIGH);
  digitalWrite(pMotorR2, LOW); 
  
  nSpeed = 0;
}

// Turn on Left and Right motors at different speeds.
void loop()
{
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
  digitalWrite(pMotorL1, HIGH);
  digitalWrite(pMotorL2, LOW);
  analogWrite(pMotorLSpeedCtrl,200);
  digitalWrite(pMotorR1, HIGH);
  digitalWrite(pMotorR2, LOW);
  analogWrite(pMotorRSpeedCtrl,200);  
  delay(2000);   
  // Set motor A and B directions, CW, speed 70.
  digitalWrite(pMotorL1, HIGH);
  digitalWrite(pMotorL2, LOW);
  analogWrite(pMotorLSpeedCtrl,50);
  digitalWrite(pMotorR1, HIGH);
  digitalWrite(pMotorR2, LOW);
  analogWrite(pMotorRSpeedCtrl,50);  
  delay(2000);   
}
