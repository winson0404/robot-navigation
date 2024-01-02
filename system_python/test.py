import serial
import time
from utils import recvFromArduino, sendToArduino, waitForArduino, list_to_bytearray, bytearray_to_int, postprocess,structure_data

if __name__ == "__main__":
    port_name = "/dev/ttyACM0"
    baud_rate = 9600
    serial_port = serial.Serial(port_name, baud_rate)
    print(f"Serial port opened {port_name} with baud rate {baud_rate}")

    start_marker = 60
    end_marker = 62
    
    waitForArduino(serial_port)
    
    # send_data = b"<test>"
    
    mode = "w" # or "r"
    
    
    while True:
        if mode == "w":
            print("Sending data to Arduino")
            task = 2
            data_size = [2,2]
            data = [3152, 2431]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            mode = "r"
            
        if mode == "r":
            while True:
                if serial_port.in_waiting > 0:
                    # print("Available byte count: ", serial_port.in_waiting)
                    data, byte_received = recvFromArduino(serial_port, start_marker, end_marker)
                    # result = bytearray_to_int(list_to_bytearray(data))\
                    if byte_received != 10:
                        print("Incorrect bytes received, count", byte_received)
                        #clear input buffer
                        serial_port.reset_input_buffer()
                        mode = "w"
                        break
                    task, results = postprocess(data, byte_received)
                    # breakpoint()
                    print("Message received (0):", results[0])
                    print("Message received (1):", results[1])
                    if results[0] == 1234 and results[1] == 5678:
                        mode = "w"
                        break
                    
    
