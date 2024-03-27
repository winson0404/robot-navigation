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
    velocity = 150
    radian = 1
    while True:
        try:
            # velocity = int(float(input("Enter velocity): "))*100)
            user_input = input("move and time: ")
            move, delay = user_input.split(" ")
            # delay(int(delay))
            delay = int(delay)
            # print("2. Retrieve sensor data")
            
            # move = "r"
            
            if move == "v":
                
                print(f"Sending data of velocity: {velocity/100} and radian: {0.0} with delay: {delay}")
                task = 30
                data_size = [2, 2, 2]
                if delay < 0:
                    delay = -delay
                data = [velocity*100, 0, delay]
                send_data = structure_data(start_marker, end_marker, task, data_size, data)
                # breakpoint()
                sendToArduino(serial_port, send_data)
                print("Data sent")
                # delay for 200 milliseconds
            elif move == "r":
                if delay < 0:
                    radian = -1.5
                    delay = -delay
                else:
                    radian = 1.5
                # print("2. Retrieve sensor data")
                
                print(f"Sending data of velocity: {0} and radian: {radian} with delay: {delay}")
                task = 30
                data_size = [2, 2, 2]
                data = [0, int(radian*100), delay]
                send_data = structure_data(start_marker, end_marker, task, data_size, data)
                # breakpoint()
                sendToArduino(serial_port, send_data)
                print("Data sent")
                # delay for 200 milliseconds

        except:
            print("Invalid input")
            breakpoint()

if __name__ == "__main__":
    calibrate_velocity()