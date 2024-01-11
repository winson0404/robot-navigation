from dataclasses import dataclass

@dataclass
class Packet:
    packet_length:int
    task: int
    num_data: int
    data_length: list[int]
    data: list[int]
    checksum: bytes

def reset_packet(p, packet_length, task, num_data, data_length, data, checksum):
    p.packet_length = packet_length
    p.task = task
    p.num_data = num_data
    p.data_length = data_length
    p.data = data
    p.checksum = checksum

def test1():
    global p
    print("Hello World!")
    print(p.packet_length)
    print(p.task)
    print(p.num_data)
    print(p.data_length)
    print(p.data)
    print(p.checksum)
    # reset_packet(p, 11,22,33,[44,55,66],[77,88,99],b"5678")
    p = Packet(11,22,33,[44,55,66],[77,88,99],b"5678")

if __name__ == "__main__":
    global p
    p = Packet(1,2,3,[4,5,6],[7,8,9],b"1234")
    print("Hello World!")
    test1()
    print(p.packet_length)
    print(p.task)
    print(p.num_data)
    print(p.data_length)
    print(p.data)
    print(p.checksum)
    