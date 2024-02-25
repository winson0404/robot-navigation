#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
# import serial
from .comms.uart import UART_Serial
from .utils import _constants as constant
from .utils.helper_type_conversion import list_to_bytearray, bytearray_to_int
from .utils.helper_comm import structure_data
from custom_interfaces.msg import SensorStatus, MovementCommand
from custom_interfaces.srv import ControlMovement
from typing import List, Tuple
import time
import os

import random

class CommNode(Node):
    def __init__(self, ser: UART_Serial):
        super().__init__('comm_node')
        self.ser = ser
        self.isReceivingComm = False
        self.gotTask = False
        self.sensor_data_publisher = self.create_publisher(SensorStatus, 'sensor_status', 10)
        self.movement_command_subscriber = self.create_subscription(MovementCommand, 'movement_command_status', self.sensor_data_callback, 10)
        self.get_logger().info(f'CommNode has been initialized on {constant.SERIAL_PORT} with baud rate {constant.BAUD_RATE}')
        self.velocity = 0.0
        self.radian = 0.0
        self.delay = 0
        
        self.mode = os.environ['NAVIGATION_MODE']
        self.map = os.environ['MAP']
        self.destination = os.environ['DESTINATION']
        
        if self.mode == constant.NAVIGATION_MODE_ROAMING:
            
            self.create_timer(0.3, self.fetch_sensor_callback)
            
        else:
            self.run_predefined_path()
            
        self.srv = self.create_service(ControlMovement, 'control_movement', self.move_robot_callback)
        print("Environment variable: ", os.environ['NAVIGATION_MODE'])
        
        
    def run_predefined_path(self)->None:
        predefined_path = self.predefined_path()
        print(f"Running predefined path with MAP: {self.map}, DESTINATION: {self.destination}")
        for movement_set in predefined_path:
            velocity, radian, delay = movement_set
            velocity = int(velocity*100)
            radian = int(radian*100)
            print(f"Moveset: velocity: {velocity}, radian: {radian}, delay: {delay}")
            
            task = constant.MOTOR_MOVE
            data_size = [2, 2, 2]
            data = [velocity, radian, delay]
            send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, task, data_size, data)
            
            self.ser.send_bytearray(send_data)
            self.ser.receive_acknowledgement()
            time.sleep(1)
            print("Done move")
            
        # quit node
        rclpy.shutdown()
            
    def predefined_path(self)->List[Tuple[float, float, int]]:
        # if map is 1, destination is 1, go to predefined path 1
        # if map is 1, destination is 2, go to predefined path 2
        
        velocity = 150.0
        small_clockwise_radian = -0.52
        small_counter_clockwise_radian = 0.52
        medium_clockwise_radian = -1.04
        medium_counter_clockwise_radian = 1.04
        big_clockwise_radian = -1.57
        big_counter_clockwise_radian = 1.57
        # return (velocity, radian, time in ms)
        
        decision_map = {
            constant.DECISION_MOVE_FRONT: (velocity, 0.0, 500),
            constant.DECISION_SMALL_ROTATE_COUNTER_CLOCKWISE: (0.0, small_counter_clockwise_radian, 190),
            constant.DECISION_SMALL_ROTATE_CLOCKWISE: (0.0, small_clockwise_radian, 190),
            constant.DECISION_MEDIUM_ROTATE_COUNTER_CLOCKWISE: (0.0, medium_counter_clockwise_radian, 280),
            constant.DECISION_MEDIUM_ROTATE_CLOCKWISE: (0.0, medium_clockwise_radian, 280),
            constant.DECISION_MEDIUM_ROTATE_BOTH_DIRECTION: (0.0, random.choice([medium_counter_clockwise_radian, medium_clockwise_radian]), 280),
            constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE: (0.0, big_counter_clockwise_radian, 500),
            constant.DECISION_BIG_ROTATE_CLOCKWISE: (0.0, big_clockwise_radian, 500),
            constant.DECISION_BIG_ROTATE_BOTH_DIRECTION: (0.0, random.choice([big_counter_clockwise_radian, big_clockwise_radian]), 500)
        }
        
        if self.map == constant.MAP1:
            if self.destination == constant.DESTINATION1:
                return [
                        decision_map[constant.DECISION_MOVE_FRONT], 
                        decision_map[constant.DECISION_MOVE_FRONT], 
                        decision_map[constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE], 
                        decision_map[constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE], 
                        decision_map[constant.DECISION_MOVE_FRONT]
                    ]
            else:
                return [
                        decision_map[constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE], 
                        decision_map[constant.DECISION_MOVE_FRONT], 
                        decision_map[constant.DECISION_BIG_ROTATE_CLOCKWISE], 
                        decision_map[constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE], 
                        decision_map[constant.DECISION_BIG_ROTATE_CLOCKWISE]
                    ]

            
        else:
            return [];
    def sensor_data_callback(self, msg: MovementCommand)->None:
        self.velocity = msg.velocity
        self.radian = msg.radian
        self.delay = msg.delay

    def fetch_sensor_callback(self)->None:
        """Request for sensor data from Arduino every 200 milliseconds if no task:
        
            data protocol:
            Byte_Size     Sequence                 Content
                2            1         ultrasonic data (float*100 => int16_t)
                2            2         front infared ray data (float*100 => int16_t)
                2            3         left infared ray data (float*100 => int16_t)
                2            4         right infared ray data (float*100 => int16_t)
        """
        
        self.get_logger().info(f"\ntrying to fetching sensor data==========================")
        if self.isReceivingComm == False and self.gotTask == False:
            start_time = time.time()
            self.isReceivingComm = True
            send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, constant.SENSOR_DATA_REQ, [], [])
            self.ser.send_bytearray(send_data)
            if (self.ser.receive_acknowledgement() != constant.ACKNOWLEDGEMENT_SUCCESS):
            # if False:
                self.isReceivingComm = False
                msg = SensorStatus()
                msg.front_us = -100.0
                msg.front_ir = -100
                msg.left_ir = -100
                msg.right_ir = -100
                self.sensor_data_publisher.publish(msg)
                print("Failed to request for sensor data")
                # stop node for 500 ms
                # time.sleep(0.5)
                return
            task, results, status = self.receive_sensor_data_handler()
            # self.ser.send_acknowledgement(constant.STARTMARKER, constant.ENDMARKER, status)
            
            # if False:
            # if status != constant.ACKNOWLEDGEMENT_SUCCESS:
                # self.isReceivingComm = False
                # msg = SensorStatus()
                # msg.front_us = -100.0
                # msg.front_ir = -100
                # msg.left_ir = -100
                # msg.right_ir = -100
                # self.sensor_data_publisher.publish(msg)
                # print(f"Failed to receive sensor data: status: {status}")
                # # stop node for 500 ms
                # # time.sleep(0.5)
                
                
            # else:
            try:
                msg = SensorStatus()
                msg.front_us = float(results[0])/100
                msg.front_ir = results[1]
                msg.left_ir = results[2]
                msg.right_ir = results[3]

                self.sensor_data_publisher.publish(msg)
                self.isReceivingComm = False
                self.get_logger().info(f"Time taken to fetch sensor data: {(time.time() - start_time)*1000} ms")
            except Exception as e:
                print(e)
                
                self.isReceivingComm = False
                msg = SensorStatus()
                msg.front_us = -100.0
                msg.front_ir = -100
                msg.left_ir = -100
                msg.right_ir = -100
                self.sensor_data_publisher.publish(msg)
                print("Failed to publish sensor data")
                # stop node for 500 ms
                # time.sleep(0.5)
            # stop node for 200 ms to wait for response
            time.sleep(0.05) #
            
            self.control_robot()
            self.ser.receive_acknowledgement()
            time.sleep(2000)
        else:
            print(f"Doing motor task: {self.gotTask}")
            # check queue for subscribers
            
        # sleep for 2 seconds
        # time.sleep(2)
        self.get_logger().info(f"Done fetching sensor data ==========================")
        
    def control_robot(self):
        self.get_logger().info(f"Sending motor task: velocity: {self.velocity}, radian: {self.radian}, delay: {self.delay}+++++++++++++++++++++")
        
        velocity = int(self.velocity*100)
        radian = int(self.radian*100)
        
        task = constant.MOTOR_MOVE
        data_size = [2, 2, 2]
        data = [velocity, radian, self.delay]
        send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, task, data_size, data)
        
        self.ser.send_bytearray(send_data)
        
    def move_robot_callback(self, request, response):
        self.get_logger().info(f"Initiate motor task+++++++++++++++++++++")
        self.gotTask = True
        self.get_logger().info(f"Got Receiving Task: {self.isReceivingComm}")
        self.get_logger().info(f"Received request: {request}")
        velocity = int(request.velocity*100)
        radian = int(request.radian*100)
        delay = request.delay
        
        task = constant.MOTOR_MOVE
        data_size = [2, 2, 2]
        data = [velocity, radian, delay]
        send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, task, data_size, data)
        
        self.ser.send_bytearray(send_data)
        
        # response.status = self.ser.receive_acknowledgement() == 0
        response.status = True
        self.gotTask = False
        
        self.get_logger().info(f"Done task, response: {response.status}++++++++++++++")
        
        return response

    def receive_sensor_data_handler(self)->Tuple[int, List[int], int]:
        start_time = time.time()
        while True:
            if self.ser.ser.in_waiting > 0:
                data, byteCount, status = self.ser.receive_data(constant.STARTMARKER, constant.ENDMARKER)
                if status == constant.ACKNOWLEDGEMENT_FAIL:
                    return None, None, status
                # print(f"status: {status}")
                task, results = self.ser.postprocess(data, byteCount)
                if len(results) == 0:
                    print("length results == 0")
                    return None, None, constant.ACKNOWLEDGEMENT_FAIL
                return task, results, status
            
            # time out 400 ms
            if time.time() - start_time > constant.TIMEOUT:
                print("Receive Sensor Data Timeout")
                return None, None, constant.ACKNOWLEDGEMENT_FAIL
        
    
    def publish_sensor_data(self):
        msg = SensorStatus()
        msg.front_ir = 0.1230000000000
        msg.left_ir = 0.4560000000000
        msg.right_ir = 0.7890000000000
        msg.front_us = 1.2340000000000
        self.sensor_data_publisher.publish(msg)
        


def main(args=None):
    ser = UART_Serial(constant.SERIAL_PORT, constant.BAUD_RATE)
    rclpy.init(args=args)
    node = CommNode(ser)
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()