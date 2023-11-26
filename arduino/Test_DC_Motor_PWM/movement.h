#ifndef MOVEMENT_FUNCTIONS_H
#define MOVEMENT_FUNCTIONS_H
#include <Arduino.h>



namespace movement{
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

    void movement_setup();
    void movement_handler(char* data, char &packet_length, char &task);
    void adjustSpeed(int motor, int speed);

    void spinBack(int motor);
    void spinFront(int motor);
    void spinStop(int motor);

    void moveFront();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveStop();




}


#endif
