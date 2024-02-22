#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import random
# import serial
from .comms.uart import UART_Serial
from .utils import _constants as constant
from .utils.helper_type_conversion import list_to_bytearray, bytearray_to_int
from .utils.helper_comm import structure_data
from custom_interfaces.msg import SensorStatus, InferenceResult
from custom_interfaces.srv import ControlMovement
from typing import List, Tuple
import time

class BrainNode(Node):
    def __init__(self):
        super().__init__('comm_node')
        # create parameter for mode, map and destination
        
        self.declare_parameter('mode', constant.NAVIGATION_MODE_ROAMING)
        self.declare_parameter('map', constant.MAP1)
        self.declare_parameter('destination', constant.DESTIONATION1)
        

        self.front_us = -100.0
        self.front_ir = -100
        self.left_ir = -100
        self.right_ir = -100
        
        self.model_result = -1
        self.camera_data = 0
        
        self.sensor_data_subscriber = self.create_subscription(SensorStatus, 'sensor_status', self.sensor_data_callback, 10)
        self.model_result_subscriber = self.create_subscription(InferenceResult, 'inference_result', self.model_result_callback, 10)
        self.move_robot_client = self.create_client(ControlMovement, 'control_movement')
        
        # read from sensor every 0.2 seconds
        # print(self, self.get_parameter('mode').get_parameter_value())
        if self.get_parameter('mode').get_parameter_value().integer_value == constant.NAVIGATION_MODE_ROAMING:
            # print("Roaming mode")
            self.create_timer(0.2, self.control_robot)
            
        elif self.get_parameter('mode').get_parameter_value().integer_value == constant.NAVIGATION_MODE_PREDEFINED_PATH:
            # print("Predefined path")
            self.run_predefined_path()
            
        else:
            print("Unknown mode")
    
    def control_robot(self)->None:
        client = self.create_client(ControlMovement, 'control_movement')
        while not client.wait_for_service(timeout_sec=None):
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
            self.get_logger().info(f'Service call stauts: {response.status}')
    
    def run_predefined_path(self)->None:
        predefined_path = self.predefined_path()
        for movement_set in predefined_path:
            velocity, radian, time = movement_set
            client = self.create_client(ControlMovement, 'control_movement')
            while not client.wait_for_service(timeout_sec=None):
                self.get_logger().info('Waiting for ControlMovement service...')
                
            if velocity == 0.0:
                # delay for time
                time.sleep(time)
                
            request = ControlMovement.Request()
            request.velocity, request.radian = movement_set
            
            future = client.call_async(request)
            
            time.sleep(0.3)
        
    
    def make_movement_decisions(self)->Tuple[float, float]:
        
        # if no obstacle, move front
        us_threshold = 20 # in cm
        velocity = 100
        small_clockwise_radian = -0.5
        small_counter_clockwise_radian = 0.5
        medium_clockwise_radian = -1.5
        medium_counter_clockwise_radian = 1.5
        big_clockwise_radian = -3
        big_counter_clockwise_radian = 3
        us_reading = us_threshold > self.front_us # 0 means obstacle, 1 means no obstacle
        front_ir_reading = not self.front_ir # converts to 0 means obstacle, 1 means no obstacle
        sensor_conclusion = -1
        model_conclusion = -1
        sensor_decision = -1
        model_decision = -1
        
        
        # obtain sensor conclusion based on 0 0 0 0, 0 0 0 1 and so on until 1 1 1 1
        
        if us_reading == 0 and front_ir_reading == 0 and self.left_ir == 0 and self.right_ir == 0:
            sensor_conclusion = constant.NO_OBSTACLE
            sensor_decision = constant.DECISION_MOVE_FRONT
            
        elif us_reading == 0 and front_ir_reading == 0 and self.left_ir == 0 and self.right_ir == 1:
            sensor_conclusion = constant.SMALL_RIGHT
            sensor_decision = constant.DECISION_SMALL_ROTATE_COUNTER_CLOCKWISE
            
        elif us_reading == 0 and front_ir_reading == 0 and self.left_ir == 1 and self.right_ir == 0:
            sensor_conclusion = constant.SMALL_LEFT
            sensor_decision = constant.DECISION_SMALL_ROTATE_CLOCKWISE
            
        elif us_reading == 0 and front_ir_reading == 0 and self.left_ir == 1 and self.right_ir == 1:
            sensor_conclusion = constant.SMALL_LEFT_RIGHT
            sensor_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
        ######################################################################################################
        elif us_reading == 0 and front_ir_reading == 1 and self.left_ir == 0 and self.right_ir == 0:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
            
        elif us_reading == 0 and front_ir_reading == 1 and self.left_ir == 0 and self.right_ir == 1:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE
            
        elif us_reading == 0 and front_ir_reading == 1 and self.left_ir == 1 and self.right_ir == 0:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_CLOCKWISE
            
        elif us_reading == 0 and front_ir_reading == 1 and self.left_ir == 1 and self.right_ir == 1:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
        ######################################################################################################
        elif us_reading == 1 and front_ir_reading == 0 and self.left_ir == 0 and self.right_ir == 0:
            sensor_conclusion = constant.SMALL_MIDDLE
            sensor_decision = constant.DECISION_MOVE_FRONT
            
        elif us_reading == 1 and front_ir_reading == 0 and self.left_ir == 0 and self.right_ir == 1:
            sensor_conclusion = constant.MEDIUM_RIGHT
            sensor_decision = constant.DECISION_MOVE_FRONT
            
        elif us_reading == 1 and front_ir_reading == 0 and self.left_ir == 1 and self.right_ir == 0:
            sensor_conclusion = constant.MEDIUM_LEFT
            sensor_decision = constant.DECISION_MOVE_FRONT
            
        elif us_reading == 1 and front_ir_reading == 0 and self.left_ir == 1 and self.right_ir == 1:
            sensor_conclusion = constant.FULL_OBSTACLE
            sensor_decision = constant.DECISION_MOVE_FRONT
        ######################################################################################################
        elif us_reading == 1 and front_ir_reading == 1 and self.left_ir == 0 and self.right_ir == 0:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
            
        elif us_reading == 1 and front_ir_reading == 1 and self.left_ir == 0 and self.right_ir == 1:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE
            
        elif us_reading == 1 and front_ir_reading == 1 and self.left_ir == 1 and self.right_ir == 0:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_CLOCKWISE
            
        elif us_reading == 1 and front_ir_reading == 1 and self.left_ir == 1 and self.right_ir == 1:
            sensor_conclusion = constant.EDGE
            sensor_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
        ######################################################################################################
        else:
            self.get_logger().info(f"Unknown sensor conclusion: {us_reading}, {front_ir_reading}, {self.left_ir}, {self.right_ir}")
            sensor_conclusion = constant.FULL_OBSTACLE
            sensor_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
            
        # obtain model conclusion
        
        if self.model_result == -1:
            model_conclusion = constant.FULL_OBSTACLE
            model_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
            
        elif self.model_result == 0:
            model_conclusion = constant.NO_OBSTACLE
            model_decision = constant.DECISION_MOVE_FRONT
            
        elif self.model_result == 1:
            model_conclusion = constant.SMALL_LEFT
            model_decision = constant.DECISION_SMALL_ROTATE_CLOCKWISE
            
        elif self.model_result == 2:
            model_conclusion = constant.SMALL_MIDDLE
            model_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
        
        elif self.model_result == 3:
            model_conclusion = constant.SMALL_RIGHT
            model_decision = constant.DECISION_SMALL_ROTATE_COUNTER_CLOCKWISE
            
        elif self.model_result == 4:
            model_conclusion = constant.FULL_OBSTACLE
            model_decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
            
        elif self.model_result == 5:
            model_conclusion = constant.MEDIUM_LEFT
            model_decision = constant.DECISION_MEDIUM_ROTATE_CLOCKWISE
            
        elif self.model_result == 6:
            model_conclusion = constant.MEDIUM_RIGHT
            model_decision = constant.DECISION_MEDIUM_ROTATE_COUNTER_CLOCKWISE
            
        else:
            self.get_logger().info(f"Unknown model result: {self.model_result}")
            model_conclusion = constant.FULL_OBSTACLE
            model_decision = constant.DECISION_MOVE_FRONT
        
        # Cast to hierachy
        model_hierachy = -1
        sensor_hierachy = -1
        
        # get sensor hierachy
        if sensor_conclusion == constant.NO_OBSTACLE:
            sensor_hierachy = 0
            
        elif sensor_conclusion == constant.SMALL_LEFT or sensor_conclusion == constant.SMALL_RIGHT:
            sensor_hierachy = 1
            
        elif sensor_conclusion == constant.SMALL_LEFT_RIGHT:
            sensor_hierachy = 2
            
        elif sensor_conclusion == constant.SMALL_MIDDLE:
            sensor_hierachy = 3
            
        elif sensor_conclusion == constant.MEDIUM_LEFT or sensor_conclusion == constant.MEDIUM_RIGHT:
            sensor_hierachy = 4
            
        elif sensor_conclusion == constant.EDGE or sensor_conclusion == constant.FULL_OBSTACLE:
            sensor_hierachy = 5
        
        
        # get model hierachy
        if model_conclusion == constant.NO_OBSTACLE:
            model_hierachy = 0
            
        elif model_conclusion == constant.SMALL_LEFT or model_conclusion == constant.SMALL_RIGHT:
            model_hierachy = 1
            
        elif model_conclusion == constant.SMALL_LEFT_RIGHT:
            model_hierachy = 2

        elif model_conclusion == constant.SMALL_MIDDLE:
            model_hierachy = 3
            
        elif model_conclusion == constant.MEDIUM_LEFT or sensor_conclusion == constant.MEDIUM_RIGHT:
            model_hierachy = 4
            
        elif model_conclusion == constant.EDGE or sensor_conclusion == constant.FULL_OBSTACLE:
            model_hierachy = 5

        # make decision based on hierachy
        decision = constant.DECISION_BIG_ROTATE_BOTH_DIRECTION
        if model_hierachy < sensor_hierachy:
            decision = model_decision
        elif model_hierachy > sensor_hierachy:
            decision = sensor_decision
        else:
            decision = model_decision
        
        if decision == constant.DECISION_MOVE_FRONT:
            return velocity, 0.0
        
        elif decision == constant.DECISION_SMALL_ROTATE_COUNTER_CLOCKWISE:
            return 0.0, small_counter_clockwise_radian
        
        elif decision == constant.DECISION_SMALL_ROTATE_CLOCKWISE:
            return 0.0, small_clockwise_radian
        
        elif decision == constant.DECISION_MEDIUM_ROTATE_COUNTER_CLOCKWISE:
            return 0.0, medium_counter_clockwise_radian
        
        elif decision == constant.DECISION_MEDIUM_ROTATE_CLOCKWISE:
            return 0.0, medium_clockwise_radian
        
        elif decision == constant.DECISION_MEDIUM_ROTATE_BOTH_DIRECTION:
            return 0.0, random.choice([medium_counter_clockwise_radian, medium_clockwise_radian])
        
        elif decision == constant.DECISION_BIG_ROTATE_COUNTER_CLOCKWISE:
            return 0.0, big_counter_clockwise_radian
        
        elif decision == constant.DECISION_BIG_ROTATE_CLOCKWISE:
            return 0.0, big_clockwise_radian
        
        elif decision == constant.DECISION_BIG_ROTATE_BOTH_DIRECTION:
            return 0.0, random.choice([big_counter_clockwise_radian, big_clockwise_radian])
        
        else:
            self.get_logger().info(f"Void decision, stoping robot")
            return 0.0, 0.0

    def predefined_path(self)->List[Tuple[float, float]]:
        # if map is 1, destination is 1, go to predefined path 1
        # if map is 1, destination is 2, go to predefined path 2
        if self.get_parameter('map').get_parameter_value() == constant.MAP1:
            if self.get_parameter('destination').get_parameter_value() == constant.DESTIONATION1:
                return [(100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90)]
            else:
                return [(100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90)]
            
        elif self.get_parameter('map').get_parameter_value() == constant.MAP1:
            if self.get_parameter('destination').get_parameter_value() == constant.DESTIONATION1:
                return [(100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90)]
            else:
                return [(100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90)]
            
        elif self.get_parameter('map').get_parameter_value() == constant.MAP1:
            if self.get_parameter('destination').get_parameter_value() == constant.DESTIONATION1:
                return [(100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90)]
            else:
                return [(100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90), (100, 0), (0, 90)]
            
        else:
            return [];
            
    
    def sensor_data_callback(self, msg: SensorStatus)->None:
        self.front_us = msg.front_us
        self.front_ir = msg.front_ir
        self.left_ir = msg.left_ir
        self.right_ir = msg.right_ir
        # self.get_logger().info(f"Sensor data received: {self.front_us}, {self.front_ir}, {self.left_ir}, {self.right_ir}")
        
    def model_result_callback(self, msg: InferenceResult)->None:
        self.model_result = msg.model_result
    
    
def main(args=None):
    rclpy.init(args=args)
    node = BrainNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()