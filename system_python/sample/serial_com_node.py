#!/usr/bin/env python3      # Only use python 3.
'''
Description:    Code to handle outgoing and incoming data packets from
                serial port.
Author:         Fabian Kung
Last modified:  25 Dec 2023
'''

import rclpy                # Library for ROS2 in python. Note that this
                            # is a package, i.e. it is a folder named 
                            # 'rclpy' and contains a number of *.py 
                            # files which are the various modules making 
                            # up the library.
from rclpy.node import Node
#from example_interfaces.msg import String # Import String datatype for messaging.
from wheelrobot_interface.msg import RCStateRaw
from wheelrobot_interface.msg import RCCommand

import serial


class SerialComNode(Node):  # Create a class, inherited from the Node class.

    ReadData = []           # Class level variable, captured data from serial port.

    def __init__(self):     # Constructor.
        super().__init__("serial_com_node") # Create a node, initialize node name. 
                            # Basically here we run all the parent's constructors. 
                            # In python, the super() function is used to give
                            # access to methods and properties of a parent or
                            # sibling class.
        #self.counter = 0    
        self.get_logger().info("Serial COM node starting up...")
        
        # Create a publisher using the interface RCStateRaw,
        # with topic name "t_rc_status", and queue size of 8.
        self.publisher_ = self.create_publisher(RCStateRaw, "t_rc_status", 8)

        # Create a subscriber using the interface RCCommand, with topic name 
        # "t_rc_command" and queue size of 8.                    
        self.subscriber_ = self.create_subscription(RCCommand, 
                                                    "t_rc_command",self.Subscriber_Callback,8)

        # Variables for Robot Controller (RC) serial interface.
        #self.RC_TX_Busy = False         # TX busy flag. Set to 'true' to
                                        # initiate sending of command to RC.
        #self.utf8TXCommandString = ""   # Command string to send to RC.
        #self.TXCount = 0                # TX count.

        # Create a timer
        # Timer period = 50 msec
        self.create_timer(0.05,self.Timer_Callback)
        
        # Create a serial port handle. Settings:
        # Baud rate = 38400 (for Arduino)
        #             115200 (for custom robotic controller)
        # Stop bit = 1
        # Parity = None
        # Byte length = 8 bits
        # Read timeout = 10 msec. External hardware usually responds
        # within 5 msec.
        # Write timeout = 10 msec.
        self.sport = serial.Serial('/dev/ttyUSB0', 115200) # USB-to-serial converter
        #self.sport = serial.Serial('/dev/ttyACM0', 38400) # Arduino Micro
        self.sport.timeout = 0.01 # Read timeout.
        self.sport.write_timeout = 0.01 # Write timeout.
        self.sport.reset_input_buffer() # Clear receive buffer first.

    """
    Callback function for timer, 
    This function contains routines to read data packet from external Robot Controller (RC).
    There are two kinds of data packets:
    1. Periodic RC status packet which report the hardware status of the robotic platform
       with format like this:
       Byte  Content
       0     'b'
       1      Velocityr.1 - MSB, or bits 15-8 of 16 bits signed integer representing right wheel 
                           velocity in (rev/sec) x 100 + 1024.
       2      Velocityr.0 - bits 7-0
       3      Velocityl.1 - MSB, or bits 15-8 of 16 bits signed integer representing left wheel 
                           velocity in (rev/sec) x 100 + 1024.
       4      Velocityl.0 - bits 7-0      
       5      Heading.1 -  MSB, or bits 15-8 of 16 bits signed integer representing heading
                           or direction of the robotic platform.
       6      Heading.0 -  bits 7-0 
       7      Hardware status flags - Reserved
       8      Hardware status flags - Reserved      
       9      Sensor1.1 -  bits 15-8
       10     Sensor1.0 -  bits 7-0
       11     Sensor2.1 -  bits 15-8
       12     Sensor2.0 -  bits 7-0
       13     Sensor3.1 -  bits 15-8
       14     Sensor3.0 -  bits 7-0
       15     Checksum byte, using sum complement method.

    2. Acknowledgement from RC when receiving a command packet from ROS2. This is just 1 byte,
       either ACK or NAK. (Not implementing for now)
    """
    def Timer_Callback(self):   
        if self.sport.is_open == True:
            bytetoread = self.sport.in_waiting
            if bytetoread > 20:  # At least 20 bytes
                ReadData = self.sport.read(bytetoread)
                self.sport.reset_input_buffer()
                #self.get_logger().info(str(ReadData))  # Echo receive data to terminal.
                    
                # Verify that the checksum value is correct. 
                # Here we are using the sum complement method, So all the bytes in the
                # received buffer should adds up to 256 (lower 8-bits is 0) or 0.
                sum = 0
                for data in ReadData:
                    sum = sum + data
                sum = sum & 0x00FF      # Mask out all bits except lower byte.

                if (sum == 0) or (sum == 256): # Make sure checksum is correct.
                    if ReadData[0] == 0x62:    # Compare with character 'b'.
                        #self.get_logger().info("Checksum check " + str(sum))  
                        msg = RCStateRaw() # Note RCStateRaw is a function or method.
                        # Get distance.  Assemble the bytes into a 32-bits signed integer, something as shown
                        # below.
                        # tempdata = (ReadData[1]<<24) + (ReadData[2]<<16) + (ReadData[3]<<8) + ReadData[4]
                        # Here it is big endian, the most significant byte (MSB) is assumed to be send first.
                        # Get right wheel velocity. Convert the big endidan bytes into 16-bits signed integer. 
                        # Note that there is an offset which we must subtract off.
                        msg.velocityr = int.from_bytes([ReadData[1], ReadData[2]], byteorder='big',signed=True) - 1024
                        #self.get_logger().info("Right velocity " + str(msg.velocityr)) 
                        # Get left wheel velocity. Convert the big endidan bytes into 16-bits signed integer. 
                        # Note that there is an offset which we must subtract off.
                        msg.velocityl = int.from_bytes([ReadData[3], ReadData[4]], byteorder='big',signed=True) - 1024  
                        #self.get_logger().info("Left velocity " + str(msg.velocityl))                      
                        #self.get_logger().info("Velocity " + str(msg.velocity))  
                        # Get heading. Convert the bytes into 16-bits integer.
                        msg.heading = int.from_bytes([ReadData[5], ReadData[6]], byteorder='big',signed=True)
                        #self.get_logger().info("Heading " + str(msg.heading)) 
                        # Get range sensor output.
                        msg.sen_ranger = int.from_bytes([ReadData[9], ReadData[10]], byteorder='big',signed=False)
                        msg.sen_rangel = int.from_bytes([ReadData[11], ReadData[12]], byteorder='big',signed=False)
                        msg.sen_rangef = int.from_bytes([ReadData[13], ReadData[14]], byteorder='big',signed=False)
                        msg.hwstatus1 = ReadData[7]     # Get hardware status 1.
                        self.publisher_.publish(msg)    # Publish message.
                                                            

    def __del__(self):      # Destructor.
        if self.sport.is_open == True:    # Close the serial port.
            self.sport.close()
    
    def Subscriber_Callback(self, msg):
        # Check checksum value. The sum of all bytes in the 
        # message should adds up to 0 or 256 (lower 8-bits are zero)
        
        command = msg.command
        arg1 = msg.arg1
        arg2 = msg.arg2
        arg3 = msg.arg3
        arg4 = msg.arg4
        checksum = msg.checksum
        sum = command + arg1 + arg2 + arg3 + arg4 + checksum   # Make sure checksum is correct.
        
        #self.get_logger().info(str(sum))
        sum = sum & 0xFF                    # Mask out all bits except lower 8 bits.
        self.get_logger().info("Command received, sum= " + str(sum) )           
        if (sum == 0):                      # Make sure checksum is correct.
            if self.sport.is_open == True:  # Make sure serial port is opened.
                self.sport.write(bytes([command, arg1, arg2, arg3, arg4, checksum]))    
            

def main(args=None):
    rclpy.init(args=args)   # Initialize ROS communication library.
    node = SerialComNode()  # Instantiate class.
    rclpy.spin(node)        # Block the current node so that other process can run.
    rclpy.shutdown()        # Shutdown.

if __name__ == "__main__":  # Only run this part if this file (or module as python file
                            # is called) is invoked directly by the python intepreter.
    main()