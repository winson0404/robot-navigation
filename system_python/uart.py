from packet import Packet
import serial
import constants
from utils import *
import time


class UART_Serial:
    def __init__(self, port_name, baud_rate):
        self.ser = serial.Serial(port_name, baud_rate)
        print(f"Serial port opened {port_name} with baud rate {baud_rate}")
    
    def reset_input_buffer(self):
        self.ser.reset_input_buffer()
    
    def reset_output_buffer(self):
        self.ser.reset_output_buffer()
        
    def receive_with_start_end_marker(self)->Tuple[bytearray, int]:
        data = []
        x = "z"
        byteCount = 0
        while ord(x) != constants.startMarker:
            x = self.ser.read()
            if self.ser.in_waiting == 0:
                self.send_acknowledgement(constants.ACKNOWLEDGEMENT_FAIL)
                return data, byteCount
            
            
        while ord(x) != constants.endMarker:
            if ord(x) != constants.startMarker:
                data.append(x)
                byteCount += 1
            x = self.ser.read()
            
        self.send_acknowledgement(constants.ACKNOWLEDGEMENT_SUCCESS)
        return data, byteCount
    
    def send_with_start_end_marker(self, sendbytes:bytearray)->int:
        # breakpoint()
        self.ser.write(int.to_bytes(constants.STARTMARKER, 1, byteorder=constants.ENDIAN))
        self.ser.write(sendbytes)
        self.ser.write(int.to_bytes(constants.ENDMARKER, 1, byteorder=constants.ENDIAN))
            #wait until send finishes
        while self.ser.out_waiting > 0:
            pass
        
        ack = self.receive_acknowledgement()
        return ack
    
    def send_acknowledgement(self, response:int):
        output = [int.to_bytes(response, 1, byteorder="little")]
        self.send_with_start_end_marker(list_to_bytearray(output))
        print("Response sent")
        
    def receive_acknowledgement(self)->int:
        start_time = time.time()
        while True:
            if self.ser.in_waiting >= 3:
                response, _ = self.receive_with_start_end_marker()
                response = bytearray_to_int(response[0])
                if response == constants.ACKNOWLEDGEMENT_SUCCESS:
                    print("Response received: Success")
                    return response
                elif response == constants.ACKNOWLEDGEMENT_FAIL:
                    print("Response received: Fail")
                    return response
                else:
                    print("Response received: Unknown")
                    return response
            elif time.time() - start_time > constants.TIMEOUT:
                print("Timeout")
                return -1