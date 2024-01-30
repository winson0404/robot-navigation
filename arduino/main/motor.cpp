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
    spin_stop(LEFT_MOTOR);
    spin_stop(RIGHT_MOTOR);

    adjust_speed(LEFT_MOTOR, 255);
    adjust_speed(RIGHT_MOTOR, 255);
  }

  void motor_handler(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {

    switch (p.task)
    {
    case constants::MOTOR_MOVE:
      move_motor_task(comm_state, task_state, p);
      break;

    case constants::MOTOR_VELOCITY_REQ:
      velocity_req_task(comm_state, task_state, p);
      break;

    default:
      break;
    }
  }

  void move_motor_task(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {
    /*
    Controls the rotation and displacement of robot based on the velocity and radian received from the processor.
            Byte_Size     Sequence                 Content
                2            1         velocity (float*100 => d_int)
                2            2         radian (float*100 => d_int)
    */

    float velocity = ((float)p.data[0] )/ 100;
    float radian = ((float)p.data[1] )/ 100;
    Serial.println("Motor Task Received..");

    // move_motor_with_speed(speed);
    handle_rotate_with_radian(radian);
    delay(1000);
    handle_displacement_with_velocity(velocity);

    task_state = constants::COMMS;
  }

  void handle_rotate_with_radian(float radian)
  {
    Serial.print("Rotating with radian: ");
    Serial.println(radian);
    int degree = (radian * 180) / 3.14159;

    bool is_clockwise = degree > 0;

    int time_needed = (48.88 * (degree)) / 10;

    adjust_speed(LEFT_MOTOR, 90);
    adjust_speed(RIGHT_MOTOR, 90);

    if (is_clockwise)
      abs_rotate_clockwise();
    else
      abs_rotate_counter_clockwise();

    delay(time_needed);

    move_stop();
  }

  void handle_displacement_with_velocity(float velocity)
  {
    int speed = (int)velocity; // TODO: do some conversion to map velocity to -255 to 255
    Serial.print("moving with speed: ");
    Serial.println(speed);
    // make sure velocity is in range -255 to 255
    if (speed > 255)
      speed = 255;
    else if (speed < -255)
      speed = -255;

    if (speed > 0)
    {
      move_front(); // place holder cause motor dont accept speed to move wheel
      adjust_speed(LEFT_MOTOR, speed);
      adjust_speed(RIGHT_MOTOR, speed);
    }
    else if (speed < 0)
    {
      move_back(); // place holder cause motor dont accept speed to move wheel

      speed = -speed;
      adjust_speed(LEFT_MOTOR, speed);
      adjust_speed(RIGHT_MOTOR, speed);
    }
    else
    {
      move_stop(); // place holder cause motor dont accept speed to move wheel
      adjust_speed(LEFT_MOTOR, speed);
      adjust_speed(RIGHT_MOTOR, speed);
    }
  }

  void velocity_req_task(bool &comm_state, uint8_t &task_state, comms::packet &p)
  {
    return;
  }

  void adjust_speed(int motor, int speed)
  {

    if (motor == LEFT_MOTOR)
      analogWrite(pMotorLSpeedCtrl, speed);
    if (motor == RIGHT_MOTOR)
      analogWrite(pMotorRSpeedCtrl, speed);
  }

  void abs_rotate_clockwise()
  {
    spin_back(LEFT_MOTOR);
    spin_front(RIGHT_MOTOR);
  }

  void abs_rotate_counter_clockwise()
  {
    spin_front(LEFT_MOTOR);
    spin_back(RIGHT_MOTOR);
  }

  void move_front()
  {
    // Serial.print("Task: ");
    // Serial.println(direction);

    spin_front(LEFT_MOTOR);
    spin_front(RIGHT_MOTOR);
  }

  void move_back()
  {
    spin_back(LEFT_MOTOR);
    spin_back(RIGHT_MOTOR);
  }

  void move_stop()
  {
    spin_stop(LEFT_MOTOR);
    spin_stop(RIGHT_MOTOR);
  }

  void spin_front(int motor)
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

  void spin_back(int motor)
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

  void spin_stop(int motor)
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
