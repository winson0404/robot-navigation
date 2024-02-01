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

class CommNode(Node):
    def __init__(self, ser: UART_Serial):
        super().__init__('comm_node')
        self.ser = ser
        self.isReceivingComm = False
        self.gotTask = False
        self.sensor_data_publisher = self.create_publisher(SensorStatus, 'sensor_status', 10)
        self.get_logger().info(f'CommNode has been initialized on {constant.SERIAL_PORT} with baud rate {constant.BAUD_RATE}')
        
        self.create_timer(0.01, self.fetch_sensor_callback)
        self.srv = self.create_service(ControlMovement, 'control_movement', self.move_robot_callback)
        

    def fetch_sensor_callback(self)->None:
        """Request for sensor data from Arduino every 200 milliseconds if no task:
        
            data protocol:
            Byte_Size     Sequence                 Content
                2            1         ultrasonic data (float*100 => int16_t)
                2            2         front infared ray data (float*100 => int16_t)
                2            3         left infared ray data (float*100 => int16_t)
                2            4         right infared ray data (float*100 => int16_t)
        """
        
        self.get_logger().info(f"trying to fetching sensor data==========================")
        if self.isReceivingComm == False and self.gotTask == False:
            start_time = time.time()
            self.isReceivingComm = True
            send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, constant.SENSOR_DATA_REQ, [], [])
            self.ser.send_bytearray(send_data)
            if (self.ser.receive_acknowledgement() != constant.ACKNOWLEDGEMENT_SUCCESS):
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
            self.ser.send_acknowledgement(constant.STARTMARKER, constant.ENDMARKER, status)
            
            if status != constant.ACKNOWLEDGEMENT_SUCCESS:
                self.isReceivingComm = False
                msg = SensorStatus()
                msg.front_us = -100.0
                msg.front_ir = -100
                msg.left_ir = -100
                msg.right_ir = -100
                self.sensor_data_publisher.publish(msg)
                print(f"Failed to receive sensor data: status: {status}")
                # stop node for 500 ms
                # time.sleep(0.5)
                
                
            else:
                try:
                    msg = SensorStatus()
                    msg.front_us = float(results[0])/100
                    msg.front_ir = results[1]
                    msg.left_ir = results[2]
                    msg.right_ir = results[3]

                    self.sensor_data_publisher.publish(msg)
                    self.isReceivingComm = False
                    self.get_logger().info(f"Time taken to fetch sensor data: {(time.time() - start_time)*1000} ms")
                except:
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
        else:
            print(f"Doing motor task: {self.gotTask}")
            # check queue for subscribers
            
        # sleep for 2 seconds
        # time.sleep(2)
        self.get_logger().info(f"Done fetching sensor data ==========================")

    def move_robot_callback(self, request, response):
        self.get_logger().info(f"Initiate motor task+++++++++++++++++++++")
        self.gotTask = True
        self.get_logger().info(f"Got Receiving Task: {self.isReceivingComm}")
        self.get_logger().info(f"Received request: {request}")
        velocity = int(request.velocity*100)
        radian = int(request.radian*100)
        
        task = constant.MOTOR_MOVE
        data_size = [2, 2]
        data = [velocity, radian]
        send_data = structure_data(constant.STARTMARKER, constant.ENDMARKER, task, data_size, data)
        
        self.ser.send_bytearray(send_data)
        
        response.status = self.ser.receive_acknowledgement() == 0
        self.gotTask = False
        
        self.get_logger().info(f"Done task, response: {response.status}++++++++++++++")
        
        return response

    def receive_sensor_data_handler(self)->Tuple[int, List[int], int]:
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