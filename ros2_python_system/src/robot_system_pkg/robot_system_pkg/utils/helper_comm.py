from ._packet import Packet
from typing import List, Tuple


def structure_data(startMarker:int, endMarker:int, task:int, data_size:List[int], data:List[int])->bytearray:
    output = [int.to_bytes(startMarker, 1, byteorder="little")]
    
    # protocol task|packet_length|data_length|data
    # task
        
    checksum = 0
    packet_length = 0
    
    output.append(int.to_bytes(packet_length, 1, byteorder="little")) # packet length
    packet_length += 1

    output.append(int.to_bytes(task, 1, byteorder="little")) # task
    packet_length += 1
    
    output.append(int.to_bytes(len(data_size), 1, byteorder="little")) # num data
    packet_length += 1
    
    counter = 0
    for _ in data_size:
        _data_size = data_size[counter]
        output.append(int.to_bytes(_data_size, 1, byteorder="little"))
        packet_length += 1
        # data
        _data = data[counter]
        temp_byte = []
        _temp_data = int.to_bytes(_data, _data_size, byteorder="little", signed=True)
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
    # print(f"output: {output}")
    # print(f"bytearray(b''.join(output)): {bytearray(b''.join(output))}")
    return bytearray(b"".join(output))

def packet_to_bytearray(packet:Packet)->bytearray:
    output = []
    
    # protocol task|packet_length|data_length|data
    # task
        
    checksum = 0
    packet_length = 0
    
    output.append(int.to_bytes(packet_length, 1, byteorder="little"))
    packet_length += 1

    output.append(int.to_bytes(packet.task, 1, byteorder="little"))
    packet_length += 1
    
    output.append(int.to_bytes(len(packet.data_length), 1, byteorder="little"))
    packet_length += 1
    
    counter = 0
    for _ in packet.data_length:
        _data_size = packet.data_length[counter]
        output.append(int.to_bytes(_data_size, 1, byteorder="little"))
        packet_length += 1
        # data
        _data = packet.data[counter]
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

    
    # output.append(int.to_bytes(constants.ENDMARKER, 1, byteorder="little"))
    # output.append(int.to_bytes(checksum, 1, byteorder="little"))
    
    return bytearray(b"".join(output))