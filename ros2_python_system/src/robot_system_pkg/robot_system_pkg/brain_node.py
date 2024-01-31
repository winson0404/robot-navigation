#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
# import serial
from .comms.uart import UART_Serial
from .utils import _constants as constant
from .utils.helper_type_conversion import list_to_bytearray, bytearray_to_int
from .utils.helper_comm import structure_data
from custom_interfaces.msg import SensorStatus
from custom_interfaces.srv import ControlMovement
from typing import List, Tuple
import time

class BrainNode(Node):
    def __init__(self):
        super().__init__('comm_node')
        self.front_us = -100.0
        self.front_ir = -100
        self.left_ir = -100
        self.right_ir = -100
        self.camera_data = 0
        
        self.sensor_data_subscriber = self.create_subscription(SensorStatus, 'sensor_status', self.sensor_data_callback, 10)
        # self.move_robot_client = self.create_client(ControlMovement, 'control_movement')
        
        # read from sensor every 0.2 seconds
        # self.create_timer(0.2, self.control_robot)
    
    def control_robot(self)->None:
        client = self.create_client(ControlMovement, 'control_movement')
        while not client.wait_for_service(timeout_sec=1.5):
            self.get_logger().info('Waiting for ControlMovement service...')
            
        request = ControlMovement.Request()
        request.velocity, request.radian = self.make_movement_decisions()
        
        future = client.call_async(request)
        future.add_done_callback(self.control_robot_callback)
        
    def control_robot_callback(self, future: any)->None:
        try:
            response = future.result()
        except Exception as e:
            self.get_logger().info(f'Service call failed {e}')
        else:
            self.get_logger().info(f'Result {response.status}')
        
    def make_movement_decisions(self)->Tuple[float, float]:
        # check for error
        if self.front_us == -100.0 and self.front_ir == -100 and self.left_ir == -100 and self.right_ir == -100:
            self.get_logger().info("Sensor data not received")
            return 0.0, 0.0
        
        # if no obstacle, move front
        us_threshold = 25
    
        if self.front_us > us_threshold and self.front_ir == 1 and self.left_ir == 0 and self.right_ir == 0:
            self.get_logger().info(f"no obstacle, moving with velocity {90} and radian {0}")
            return 90.0, 0.0
        
        # if small left obstacle, turn right
        if self.front_us > us_threshold and self.front_ir == 1 and self.left_ir == 1 and self.right_ir == 0:
            self.get_logger().info(f"small left obstacle, moving with velocity {0} and radian {-0.78}")
            return 0.0, -0.78
        
        # if small right obstacle, turn left
        if self.front_us > us_threshold and self.front_ir == 1 and self.left_ir == 0 and self.right_ir == 1:
            self.get_logger().info(f"small right obstacle, moving with velocity {0} and radian {0.78}")
            return 0.0, 0.78
        
        # if big left obstacle, turn right
        if self.front_us < us_threshold and self.front_ir == 1 and self.left_ir == 1 and self.right_ir == 0:
            self.get_logger().info(f"big left obstacle, moving with velocity {0} and radian {-1.57}")
            return 0.0, -1.57
        
        # if big right obstacle, turn left
        
        if self.front_us < us_threshold and self.front_ir == 1 and self.left_ir == 0 and self.right_ir == 1:
            self.get_logger().info(f"big right obstacle, moving with velocity {0} and radian {1.57}")
            return 0.0, 1.57
        
        # if at egde, turn left
        if self.front_ir == 1:
            self.get_logger().info(f"edge detected, moving with velocity {0} and radian {3.14}")
            return 0.0, 3.14
        

        return 0.0, 0.0
        
    
    
    def sensor_data_callback(self, msg: SensorStatus)->None:
        self.front_us = msg.front_us
        self.front_ir = msg.front_ir
        self.left_ir = msg.left_ir
        self.right_ir = msg.right_ir
        self.get_logger().info(f"Sensor data received: {self.front_us}, {self.front_ir}, {self.left_ir}, {self.right_ir}")
        
    
    
    
    
    
def main(args=None):
    rclpy.init(args=args)
    node = BrainNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()