import serial
import time
from utils import *



port_name = "/dev/ttyUSB0"
baud_rate = 19200

def calibrate_rotation_angle():
    serial_port = serial.Serial(port_name, baud_rate)
    print(f"Serial port opened {port_name} with baud rate {baud_rate}")

    start_marker = 60
    end_marker = 62
    while True:
        user_input = input("Enter angle in radian: ")
        
        radian, delay = user_input.split(" ")
        radian = int(float(radian)*100)
        delay = int(delay)
        # print("2. Retrieve sensor data")
        
        move = 1
        
        if move == 1:
            
            print(f"Sending data of velocity: {0} and radian: {radian}")
            task = 30
            data_size = [2, 2, 2]
            data = [0, radian, delay]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds


if __name__ == "__main__":
    calibrate_rotation_angle()