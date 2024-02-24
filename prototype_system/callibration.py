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
        
        print("Enter radian")
        # print("2. Retrieve sensor data")
        
        move = 1
        
        if move == 1:
            velocity = int(float(input("Enter velocity: "))*100)
            radian = int(float(input("Enter angle (in radian): "))*100)
            
            print(f"Sending data of velocity: {velocity} and radian: {radian}")
            task = 30
            data_size = [2, 2]
            data = [0, radian]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds
            get_response(serial_port)


if __name__ == "__main__":
    calibrate_rotation_angle()