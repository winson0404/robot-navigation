from typing import List, Tuple
import constants
from packet import Packet

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
    
    # return [struct.pack("B", i) for i in List]

def structure_data(task:int, num_data:int, data_size:List[int], data:List[int])->Tuple(Packet, bytearray):
    output = []
    
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

    
    # output.append(int.to_bytes(endMarker, 1, byteorder="little"))
    # output.append(int.to_bytes(checksum, 1, byteorder="little"))
    
    return bytearray(b"".join(output))
    
    # return [struct.pack("B", i) for i in List]

def postprocess(data:List[bytes], byte_received:int)->Packet:
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
    p = Packet(packet_length, task, num_data, data_length, results, checksum)
    return packet


def bytearray_to_int(bytelist:bytearray)->int:
    return int.from_bytes(bytelist, byteorder="little")

def list_to_bytearray(bytelist:List[bytes])->bytearray:
    return bytearray(b"".join(bytelist))



