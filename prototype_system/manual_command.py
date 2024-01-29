import serial
import time
from utils import *
if __name__ == "__main__":
    port_name = "/dev/ttyUSB0"
    baud_rate = 9600
    serial_port = serial.Serial(port_name, baud_rate)
    print(f"Serial port opened {port_name} with baud rate {baud_rate}")

    start_marker = 60
    end_marker = 62
    
    #empty input buffer
    # serial_port.reset_input_buffer()
    # waitForArduino(serial_port)
    # breakpoint()
    # send_data = b"<test>"
    
    mode = "w" # or "r"
    
    
    start_time = time.time()
    while True:
        
        print("1. Move with velocity")
        print("2. Turn with angle")
        print("3. Retrieve sensor data")
        
        move = int(input("Enter your choice: "))
        
        if move == 1:
            velocity = int(input("Enter velocity: "))
            task = 30
            data_size = [2, 2]
            data = [velocity, 3000]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds
            get_response(serial_port)
        
        elif move == 2:
            angle = float(input("Enter angle: "))
            task = 31
            data_size = [2]
            data = [int(angle*100)]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds
            get_response(serial_port)
            
            
        elif move == 3:
            task = 20
            data_size = []
            data = []
            
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds
            get_response(serial_port)
            start_time = time.time()
            while True:
                if serial_port.in_waiting > 0:
                    print("Available byte count: ", serial_port.in_waiting)
                    data, byte_received = recvFromArduino(serial_port, start_marker, end_marker)

                    task, results = postprocess(data, byte_received)
                    
                    send_response(serial_port, start_marker, end_marker, 0)
                    print("Message received (0):", float(results[0])/100)
                    print("Message received (1):", float(results[1])/100)
                    print("Message received (2):", float(results[2])/100)
                    print("Message received (3):", float(results[3])/100)
                    # breakpoint()
                    break
                    
                    # # set timeout to 1 second
                if time.time() - start_time > 3:
                    print("Timeout")
                    mode = "w"
                    break
        
                    
                    
                        
    
