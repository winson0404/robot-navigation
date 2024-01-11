import threading
import constants
from packet import Packet
from comms import comms_listener_thread
from uart import UART_Serial

comm_mode = constants.COMM_MODE_SEND
packet_queue = []
interup = False
is_receiving_data = False
ser = UART_Serial(constants.SERIAL_PORT, constants.BAUD_RATE)

def main_process():
    # if is_receiving_data == False:
    #     if len(packet_queue) > 0:
    #         packet = packet_queue.pop(0)
    #         ser.send_with_start_end_marker(packet_to_bytearray(packet))
    #         print("Packet sent")
    pass
if __name__ == "__main__":
    # breakpoint()
    t1 = threading.Thread(target=comms_listener_thread, args=(ser, packet_queue, interup, is_receiving_data))
    
    
    
    pass