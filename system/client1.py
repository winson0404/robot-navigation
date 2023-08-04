import socket
import time

class TCP_Client():
    def __init__(self, host, port:int) -> None:
        try:
            # Create a TCP/IP socket
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Connect to the server
            self.client_socket.connect((host, port))
        except Exception as e:
            print("Error:", e)

        # finally:
        #     # Close the socket
        #     self.client_socket.close()
            
    def Send(self):
        tag = (1).to_bytes(1, byteorder='big')
        items = 3
        item_list = [22,33,44]
        message = items.to_bytes(1, byteorder='big')
        # message =  items.to_bytes(1, byteorder='big') + item_length.to_bytes(1, byteorder='big') + item.to_bytes(1, byteorder='big')
        for i in item_list:
            item_length = 1
            message += item_length.to_bytes(1, byteorder='big') + i.to_bytes(1, byteorder='big')
        message_length = len(message)
        data = tag + message_length.to_bytes(1, byteorder='big') + message
        
        self.client_socket.send(data)
        
    
    def Close(self,):
        self.client_socket.close()



if __name__ == "__main__":
    client = TCP_Client('localhost', 12345)
    while(True):
        client.Send()
        time.sleep(1)
    client.Close()
