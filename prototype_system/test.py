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
        if mode == "w":
            time.sleep(0.5)
            print("Sending data to Arduino")
            task = 3
            data_size = [2,2]
            data = [3152, 2431]
            send_data = structure_data(start_marker, end_marker, task, data_size, data)
            # breakpoint()
            sendToArduino(serial_port, send_data)
            print("Data sent")
            # delay for 200 milliseconds
            get_response(serial_port)
            mode = "r"
            start_time = time.time()
            
        if mode == "r":
            if serial_port.in_waiting > 10:
                print("Available byte count: ", serial_port.in_waiting)
                data, byte_received = recvFromArduino(serial_port, start_marker, end_marker)
                # result = bytearray_to_int(list_to_bytearray(data))\
                if byte_received != 10:
                    print("Incorrect bytes received, count", byte_received)
                    #clear input buffer
                    serial_port.reset_input_buffer()
                    # breakpoint()
                    # continue
                    print("Sending response")
                    send_response(serial_port, start_marker, end_marker, 1)
                    continue
                else:
                    task, results = postprocess(data, byte_received)
                # breakpoint()
                if results[0] == 1234 and results[1] == 5678:
                    #send response
                    print("Sending response")
                    send_response(serial_port, start_marker, end_marker, 0)
                else:
                    print("Sending response")
                    send_response(serial_port, start_marker, end_marker, 1)
                print("Message received (0):", results[0])
                print("Message received (1):", results[1])
                mode = "w"
            
            # # set timeout to 1 second
            if time.time() - start_time > 5:
                print("Timeout")
                mode = "w"
                
                
                    
    
