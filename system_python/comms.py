from packet import Packet
import constants
from uart import UART_Serial
from utils import *

def comms_listener_thread(ser:UART_Serial, p:List[Packet], interup:bool, is_receiving_data:bool)->None:
    while(True):
        if interup == False:
            if ser.ser.in_waiting() > 10:
                is_receiving_data = True
                data, byteCount = ser.receive_with_start_end_marker()
                p.append(postprocess(data, byteCount))
                is_receiving_data = False