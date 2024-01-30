#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from robot_system_pkg.comms.uart import UART_Serial
from robot_system_pkg.utils import _constants as constant
from robot_system_pkg.utils.helper_type_conversion import list_to_bytearray, bytearray_to_int
from robot_system_pkg.utils.helper_comm import structure_data
from custom_interfaces.msg import SensorStatus
from custom_interfaces.srv import ControlMovement
from typing import List, Tuple
import time

class CommNode(Node):
    def __init__(self, ser: UART_Serial):
        super().__init__('comm_node')
        self.ser = ser
        self.isReceivingComm = False
        self.gotTask = False
        self.sensor_data_publisher = self.create_publisher(SensorStatus, 'sensor_status', 10)
        self.get_logger().info(f'CommNode has been initialized on {constant.SERIAL_PORT} with baud rate {constant.BAUD_RATE}')
        
        self.create_timer(0.05, self.fetch_sensor_callback)
        self.srv = self.create_service(ControlMovement, 'control_movement', self.move_robot_callback)
        
        if self.gotTask:
            pass
        
    def fetch_sensor_callback(self)->None:
        """Request for sensor data from Arduino every 200 milliseconds if no task:
        
            data protocol:
            Byte_Size     Sequence                 Content
                2            1         ultrasonic data (float*100 => int16_t)
                2            2         front infared ray data (float*100 => int16_t)
                2            3         left infared ray data (float*100 => int16_t)
                2            4         right infared ray data (float*100 => int16_t)
        """
        
        self.get_logger().info(f"Got task: {self.gotTask}")
        if self.isReceivingComm == False and self.gotTask == False:
            start_time = time.time()
            self.isReceivingComm = True
            send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, constant.SENSOR_DATA_REQ, [], [])
            self.ser.send_bytearray(send_data)
            self.ser.receive_acknowledgement()
            task, results, status = self.receive_sensor_data_handler()
            self.ser.send_acknowledgement(constant.STARTMARKER, constant.ENDMARKER, status)
            
            msg = SensorStatus()
            msg.front_us = float(results[0])/100
            msg.front_ir = float(results[1])/100
            msg.left_ir = float(results[2])/100
            msg.right_ir = float(results[3])/100

            self.sensor_data_publisher.publish(msg)
            self.isReceivingComm = False
            self.get_logger().info(f"Time taken to fetch sensor data: {(time.time() - start_time)*1000} ms")

    def receive_sensor_data_handler(self)->Tuple[int, List[int], int]:
        while True:
            if self.ser.ser.in_waiting >= constant.SENSOR_PACKET_LENGTH:
                data, byteCount, status = self.ser.receive_data(constant.STARTMARKER, constant.ENDMARKER)
                task, results = self.ser.postprocess(data, byteCount)
                return task, results, status
    
    def move_robot_callback(self, request, response):
        self.get_logger().info(f"Received request to move robot {request.direction}")
        if request.direction == "forward":
            self.move_robot_forward()
        elif request.direction == "backward":
            self.move_robot_backward()
        elif request.direction == "left":
            self.move_robot_left()
        elif request.direction == "right":
            self.move_robot_right()
        elif request.direction == "stop":
            self.move_robot_stop()
        else:
            self.get_logger().info(f"Invalid request to move robot {request.direction}")
        return response
        
    
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