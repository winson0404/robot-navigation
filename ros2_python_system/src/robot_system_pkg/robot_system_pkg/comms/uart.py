from typing import List, Tuple
from ..utils._packet import Packet
import serial
from ..utils import _constants as constants
from ..utils.helper_type_conversion import list_to_bytearray, bytearray_to_int

import time


class UART_Serial:
    def __init__(self, port_name, baud_rate):
        self.port_name = port_name
        self.baud_rate = baud_rate
        self.ser = serial.Serial(port_name, baud_rate)
        # print(f"Serial port opened {port_name} with baud rate {baud_rate}")
    
    def reset_input_buffer(self):
        self.ser.reset_input_buffer()
    
    def reset_output_buffer(self):
        self.ser.reset_output_buffer()
        
    def receive_data(self, startMarker:int, endMarker:int)->Tuple[int, List[int], int]:
        data = []
        x = "z"  # any value that is not an end- or startMarker
        byteCount = 0  # to allow for the fact that the last increment will be one too many
        # wait for the start character
        # breakpoint()
        ndx = 0
        status = constants.ACKNOWLEDGEMENT_FAIL
        while ord(x) != startMarker:
            # print(ser.in_waiting)
            x = self.ser.read()

            if self.ser.in_waiting == 0:
                return data, byteCount, status
        

        # save data until the end marker is found
        while ord(x) != endMarker:
            if ord(x) != startMarker:
                data.append(x)
                byteCount += 1
                if ndx == 0:
                    packet_length = ord(x)
                ndx+=1;
            x = self.ser.read()
            
        status = constants.ACKNOWLEDGEMENT_SUCCESS
        
        if byteCount == 1:
            return data, byteCount, status
        
        elif byteCount != packet_length:
            print(f"Packet length miss match: packet_length: {packet_length} vs byte_count: {byteCount}")
            status = constants.ACKNOWLEDGEMENT_FAIL

        return data, byteCount, status
    

    def send_bytearray(self, sendbytes:bytearray)->None:
        # breakpoint()
        # print(sendbytes)
        self.ser.write(sendbytes)
            #wait until send finishes
        while self.ser.out_waiting > 0:
            pass

    
    def send_with_start_end_marker(self, sendbytes:bytearray)->int:
        # breakpoint()
        self.ser.write(int.to_bytes(constants.STARTMARKER, 1, byteorder=constants.ENDIAN))
        self.ser.write(sendbytes)
        self.ser.write(int.to_bytes(constants.ENDMARKER, 1, byteorder=constants.ENDIAN))
            #wait until send finishes
        while self.ser.out_waiting > 0:
            pass
        
        # ack = self.receive_acknowledgement()
        # return ack
    
    def send_acknowledgement(self, startMarker:int, endMarker:int,response:int):
        output = [int.to_bytes(startMarker, 1, byteorder="little")]
        output.append(int.to_bytes(response, 1, byteorder="little"))
        output.append(int.to_bytes(endMarker, 1, byteorder="little"))
        self.ser.write(list_to_bytearray(output))
        while self.ser.out_waiting > 0:
            pass
        
    def receive_acknowledgement(self)->int:
        start_time = time.time()
        while True:
            if self.ser.in_waiting >= 3:
                response, _, status = self.receive_data(constants.STARTMARKER, constants.ENDMARKER)
                if response == []:
                    return -1
                response = bytearray_to_int(response[0])
                if response == constants.ACKNOWLEDGEMENT_SUCCESS:
                    # print("Response received: Success")
                    return response
                elif response == constants.ACKNOWLEDGEMENT_FAIL:
                    print("Response received: Fail")
                    return response
                else:
                    print(f"Response received: Unknown ({response})")
                    return response
            elif time.time() - start_time > constants.TIMEOUT:
                print("Receive Ack Timeout")
                # breakpoint()
                return -1

    def postprocess(self, data:List[bytes], byte_received:int)->Tuple[int, List[int]]:
        # for i, byte in enumerate(data):
        # protocol checksum|packet_length|task|num_data|[data_length|data]
        counter = 0
        # breakpoint()
        checksum = 0
        # counter += 1
        try:
            
            packet_length = bytearray_to_int(data[counter])
            if packet_length != len(data):
                # breakpoint()
                return -1, []
            # print(f"packet_length: {packet_length}")
            if packet_length < 1:
                return -1, []
            counter += 1
            task = bytearray_to_int(data[counter])
            counter += 1
            num_data = bytearray_to_int(data[counter])
            counter += 1
            data_length = []
            results = []
            for i in range(num_data):
                _data_length = bytearray_to_int(data[counter])
                data_length.append(_data_length)
                counter += 1
                _data = bytearray_to_int(list_to_bytearray(data[counter:counter+_data_length]))
                results.append(_data)
                counter += _data_length
            
            checksum = bytearray_to_int(data[counter])
        except:
            breakpoint()
        return task, results