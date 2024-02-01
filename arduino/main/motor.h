#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H
#include <Arduino.h>
#include "comms.h"
#include "utils.h"

namespace motor{
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

    void motor_setup();
    uint8_t motor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p);
    void move_motor_task(bool &comm_state, uint8_t &task_state, comms::packet &p);
    void velocity_req_task(bool &comm_state, uint8_t &task_state, comms::packet &p);

    // void move_motor_with_speed(int velocity);
    void handle_rotate_with_radian(float radian);
    void handle_displacement_with_velocity(float velocity);


    
    void adjust_speed(int motor, int speed);

    void spin_back(int motor);
    void spin_front(int motor);
    void spin_stop(int motor);

    void move_front();
    void move_back();
    void abs_rotate_clockwise();
    void abs_rotate_counter_clockwise();
    void move_stop();

}


#endif
