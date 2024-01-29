#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import serial
# import torch
# from example_interfaces.msg import String


class CommNode(Node):
    def __init__(self):
        super().__init__('test_node')
        self.counter = 0
        self.create_timer(1.0, self.timer_callback)
        
        
    def timer_callback(self):
        self.counter+=1
        self.get_logger().info('Hello ROS2 ' + str(self.counter))

def main(args=None):
    rclpy.init(args=args)
    node = CommNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()