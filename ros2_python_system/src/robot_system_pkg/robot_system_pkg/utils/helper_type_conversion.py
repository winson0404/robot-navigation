from typing import List, Tuple

def bytearray_to_int(bytelist:bytearray)->int:
    return int.from_bytes(bytelist, byteorder="little", signed=True)

def list_to_bytearray(bytelist:List[bytes])->bytearray:
    return bytearray(b"".join(bytelist))