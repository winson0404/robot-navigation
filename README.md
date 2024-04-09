# Quacky
![quacky](https://github.com/winson0404/robot-navigation/assets/53987303/9cc0714f-b5cf-4c8e-a8f3-8b1ee5c43ffd)

This repository presents the source code for my robot Quacky which is my [Final Year Project](https://drive.google.com/file/d/1N3HeQ44IgfC_AOHncy7w_L8oQvX37ZI2/view?usp=sharing) titled Autonomous Navigation with Mobile Robot. 
The robot is on the Raspberry Pi 4B along with Arduino Uno as the controller, their source code is in `./arduino_system/main/` and `./ros2_python_system/src` respectively. A simple CNN model is used to assist the robot in performing navigation and obstacle avoidance, with the code and training pipeline described in `./model_training/`.

![robot_system_diagram](https://github.com/winson0404/robot-navigation/assets/53987303/407ee1bb-1787-494c-b3d5-69cba01ad3f9)


### Arduino System
The arduino system is designed based on real-time system and state machine, and communicate with the Raspberry Pi 4B through the UART with a standard protocol to construct a packet.

![state_machine](https://github.com/winson0404/robot-navigation/assets/53987303/34bc90e0-35c3-44fc-94bd-776c9622bff2)
![comms_protocol](https://github.com/winson0404/robot-navigation/assets/53987303/aa8e5ebc-a20f-4326-b24b-df6d14945e9f)

### ROS2 System
The ROS2 system running on the Raspberry Pi 4B receives information from the controller through the UART and send instruction back to it. The ROS2 system makes decision in the `brain_node`, based on the informations received from the sensors and CNN model.

![ros2_system_diagram](https://github.com/winson0404/robot-navigation/assets/53987303/f622b4e1-c1e5-4642-a8e5-f6126f64ebd3)

![quacky_front](https://github.com/winson0404/robot-navigation/assets/53987303/a750d8d6-d003-4169-ba9c-077e7f9cc32e)

![image](https://github.com/winson0404/robot-navigation/assets/53987303/6c906b19-6762-410f-9f54-3332346e977e)


## Installation

``` bash
# clone github repository
git clone https://github.com/winson0404/robot-navigation.git
cd robot-navigation

# build and run ros2 file
cd ros2_python_system
colcon build
ros2 run robot_system_pkg brain_node
ros2 run robot_system_pkg inference_node
ros2 run robot_system_pkg comm_node

# you will need to upload the arduino system file through the arduino ide
```

