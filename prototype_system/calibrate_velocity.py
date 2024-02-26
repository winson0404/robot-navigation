import serial
import time
from utils import *



port_name = "/dev/ttyUSB0"
baud_rate = 19200

def calibrate_velocity():
    serial_port = serial.Serial(port_name, baud_rate)
    print(f"Serial port opened {port_name} with baud rate {baud_rate}")

    start_marker = 60
    end_marker = 62
    while True:
        
        # velocity = int(float(input("Enter velocity): "))*100)
        user_input = int(input("Enter velocity time: "))
        # velocity, delay = user_input.split(" ")
        velocity = 150*100
        # delay(int(delay))
        delay = user_input
        # print("2. Retrieve sensor data")
        
        move = 1
        
        if move == 1:
            
            print(f"Sending data of velocity: {velocity} and radian: {0.0}")
            task = 30
            data_size = [2, 2, 2]
            data = [velocity, 0, delay]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds


if __name__ == "__main__":
    calibrate_velocity()