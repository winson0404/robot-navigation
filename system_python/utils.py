from serial import Serial
from typing import List, Tuple
import struct
import constant

def structure_data(startMarker:int, endMarker:int, task:int, data_size:List[int], data:List[int])->bytearray:
    output = [int.to_bytes(startMarker, 1, byteorder="little")]
    
    # protocol task|packet_length|data_length|data
    # task
        
    checksum = 0
    packet_length = 0
    
    output.append(int.to_bytes(packet_length, 1, byteorder="little"))
    packet_length += 1

    output.append(int.to_bytes(task, 1, byteorder="little"))
    packet_length += 1
    
    output.append(int.to_bytes(len(data_size), 1, byteorder="little"))
    packet_length += 1
    
    counter = 0
    for _ in data_size:
        _data_size = data_size[counter]
        output.append(int.to_bytes(_data_size, 1, byteorder="little"))
        packet_length += 1
        # data
        _data = data[counter]
        temp_byte = []
        _temp_data = int.to_bytes(_data, _data_size, byteorder="little")
        # breakpoint()
        for i in range(_data_size):
            output.append(_temp_data[i:i+1])
            # output.append(int.to_bytes(_data, _data_size, byteorder="little"))    
        packet_length += _data_size
        counter += 1
        # end marker
    # breakpoint()    
    output.append(int.to_bytes(checksum, 1, byteorder="little"))
    packet_length += 1
    output[1] = int.to_bytes(packet_length, 1, byteorder="little")
    #calculate checksum
    
    #initialize as - byte
    # checksum = 0
    # checksum = int.to_bytes(checksum, 1, byteorder="little")
    # checksum = sum(bytearray(b"".join(output[1:])))
    # checksum = checksum % 256
    # output[1] = int.to_bytes(0, 1, byteorder="little")
    
    output.append(int.to_bytes(endMarker, 1, byteorder="little"))
    # output.append(int.to_bytes(checksum, 1, byteorder="little"))
    
    return bytearray(b"".join(output))
    
    # return [struct.pack("B", i) for i in List]

def postprocess(data:List[bytes], byte_received:int)->Tuple[int, List[int]]:
    # for i, byte in enumerate(data):
    # protocol checksum|packet_length|task|num_data|[data_length|data]
    counter = 0
    # breakpoint()
    checksum = 0
    # counter += 1
    packet_length = bytearray_to_int(data[counter])
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
    return task, results
    # task = bytearray_to_int(data[0])
    # packet_length = bytearray_to_int(data[1])
    # # breakpoint()
    # data = data[2:]
    # counter = 0
    # output = []
    # while packet_length != 0:
    #     data_length = bytearray_to_int(data[counter])
    #     counter += 1
    #     data_seg = data[counter:counter+data_length]
    #     output.append(bytearray_to_int(list_to_bytearray(data_seg)))
    #     counter += data_length
    #     packet_length -= 1 + data_length
    
    # return task, output


def bytearray_to_int(bytelist:bytearray)->int:
    return int.from_bytes(bytelist, byteorder="little")

def list_to_bytearray(bytelist:List[bytes])->bytearray:
    return bytearray(b"".join(bytelist))

def recvFromArduino(ser: Serial, startMarker:int, endMarker:int)->List[bytes]:
    data = []
    x = "z"  # any value that is not an end- or startMarker
    byteCount = 0  # to allow for the fact that the last increment will be one too many
    # wait for the start character
    # breakpoint()
    print(f"Receiving {ser.in_waiting} bytes")
    while ord(x) != startMarker:
        # print(ser.in_waiting)
        x = ser.read()

        if ser.in_waiting == 0:
            # breakpoint()
            return data, byteCount
    
    print("Start marker found: ", ord(x))

    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            data.append(x)
            byteCount += 1
        x = ser.read()
    print("Returning from recvFromArduino")
    # breakpoint()
    return data, byteCount

def sendToArduino(ser: Serial, send_bytes:bytearray)->None:
    # ser.write(sendStr.encode('utf-8'))
    ser.write(send_bytes)
    
    #wait until send finishes
    while ser.out_waiting > 0:
        pass
def get_response(ser: Serial)->int:
    while True:
        if ser.in_waiting >= 3:
            response, _ = recvFromArduino(ser, 60, 62)
            response = bytearray_to_int(response[0])
            if response == 0:
                print("Response received: Success")
                break
            elif response == 1:
                print("Response received: Fail")
                break
        # else:
        #     print("Waiting for response")

def send_response(ser: Serial, startMarker:int, endMarker:int, response: int)->None:
    output = [int.to_bytes(startMarker, 1, byteorder="little")]
    output.append(int.to_bytes(response, 1, byteorder="little"))
    output.append(int.to_bytes(endMarker, 1, byteorder="little"))
    sendToArduino(ser, list_to_bytearray(output))
    print("Response sent")

def waitForArduino(ser: Serial)->None:

    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    global startMarker, endMarker
    
    msg = ""
    while list_to_bytearray(msg) != constant.arduino_start_message:
        # breakpoint()
        # print("Waiting for Arduino to be ready")
        while ser.in_waiting >= 9: 
            # print("Available byte count: ", ser.in_waiting)   
            msg, _ = recvFromArduino(ser, 60, 62)
            # check for empty msg
            if msg == "":
                print("Empty message received")
                continue
            break
            
    # ser.reset_input_buffer()
    print("Arduino is ready")
