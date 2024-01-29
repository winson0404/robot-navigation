from typing import NamedTuple


class Packet(NamedTuple):
    packet_length:int
    task: int
    num_data: int
    data_length: list[int]
    data: list[int]
    checksum: bytes