#!/usr/bin/env python3      # Only use python 3.
'''
Description:    Code to convert the raw robot controller raw state parameters
                into Navigation2 required messages, and publish these.
Author:         Fabian Kung
Last modified:  31 Dec 2023
'''

import rclpy                # Library for ROS2 in python. Note that this
                            # is a package, i.e. it is a folder named 
                            # 'rclpy' and contains a number of *.py 
                            # files which are the various modules making 
                            # up the library.
from rclpy.node import Node
from wheelrobot_interface.msg import RCStateRaw
from sensor_msgs.msg import Range
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
from math import cos, sin

import tf2_ros
from geometry_msgs.msg import TransformStamped

class RCStatePubNode(Node):  # Create a class, inherited from the Node class.

    ranger_msg = Range()    # Object for storing IR range sensor data, right.
    rangel_msg = Range()    # Object for storing IR range sensor data, left.
    rangef_msg = Range()    # Object for storing IR range sensor data, front.
    odom_msg = Odometry()   # Object for storing odometry information (from wheel encoders).
    dw = 0.156              # Wheel pitch, this needs to tally with URDF file.
    rw = 0.04               # Wheel radius, this needs to tally with URDF file.
    rw_dw = rw/dw
    rw_2 = rw/2.0

    def __init__(self):     # Constructor.
        super().__init__("rcstate_pub_node") # Create a node, initialize node name. 
                            # Basically here we run all the parent's constructors. 
                            # In python, the super() function is used to give
                            # access to methods and properties of a parent or
                            # sibling class.  
        self.get_logger().info("RC State Publisher node starting up...")

        # Create publisheres using the interface sensor_msgs.msg.Range,
        # with topic name "tpc_RightRange", "tpc_LeftRange", "tpc_FrontRange"
        # and queue size of 8 for each topic.
        self.publisher_r = self.create_publisher(Range, "t_rightrange", 8)
        self.publisher_l = self.create_publisher(Range, "t_leftrange", 8)
        self.publisher_t = self.create_publisher(Range, "t_frontrange", 8)

        # Create a publisher using the interface odom_msgs.msg.Odometry.
        self.publisher_odom = self.create_publisher(Odometry, "t_odom", 8)

        # Create a subscriber using the interface RCStateRaw, with topic "t_rc_status"
        # and queue size of 8.                    
        self.subscriber_ = self.create_subscription(RCStateRaw, 
                                                    "t_rc_status",self.Subscriber_Callback,8)
        
        # Setup constant parameters of IR range sensors: Sharp GP2Y0A41SK0F
        self.ranger_msg.header.frame_id = "right_rsen_link"
        self.ranger_msg.min_range = 0.04
        self.ranger_msg.max_range = 0.3  
        self.ranger_msg.radiation_type = Range.INFRARED
        self.ranger_msg.field_of_view = 0.16    # To be determined.

        self.rangel_msg.header.frame_id = "left_rsen_link"
        self.rangel_msg.min_range = 0.04
        self.rangel_msg.max_range = 0.3  
        self.rangel_msg.radiation_type = Range.INFRARED
        self.rangel_msg.field_of_view = 0.16    # To be determined.    

        self.rangef_msg.header.frame_id = "front_rsen_link"
        self.rangef_msg.min_range = 0.04
        self.rangef_msg.max_range = 0.3  
        self.rangef_msg.radiation_type = Range.INFRARED
        self.rangef_msg.field_of_view = 0.16    # To be determined.     

        # Setup constant parameters of odometry message:
        self.odom_msg.header.frame_id = 'odom'
        self.odom_msg.child_frame_id = 'base_link'      
        self.odom_msg.pose.covariance = [
            0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.1, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 99999., 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 99999., 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 99999., 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.1
        ]
        self.odom_msg.twist.covariance = [
            0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.1, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 99999., 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 99999., 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 99999., 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.1
        ]

        # Initial pose and time.
        self.x = 0.0
        self.y = 0.0
        self.theta = 0.0
        self.lasttime = self.get_clock().now()
        

    # Subscriber callback function, publish range sensor data
    def Subscriber_Callback(self, msg):

        #self.get_logger().info("Raw state received")        
        timestamp = self.get_clock().now()      # Get current time-stamp in nano seconds.
        timestamp_msg = timestamp.to_msg()      # Convert time-stamp to message.
        self.ranger_msg.header.stamp = timestamp_msg
        self.ranger_msg.range = msg.sen_ranger/1000.0   # Normalized to meter.       
        self.rangel_msg.header.stamp = timestamp_msg
        self.rangel_msg.range = msg.sen_rangel/1000.0   # Normalized to meter.
        self.rangef_msg.header.stamp = timestamp_msg
        self.rangef_msg.range = msg.sen_rangef/1000.0   # Normalized to meter.

        self.odom_msg.header.stamp = timestamp_msg
        
        # TO CHECK THIS OUT
        broadcaster = tf2_ros.TransformBroadcaster(self.get_node_names(), - )

        transform = TransformStamped()
        # Set the header information
        transform.header.stamp = timestamp_msg
        transform.header.frame_id = 'odom'
        transform.child_frame_id = 'base_link'   
        # Set the translation and orientation.
        transform.transform.translation.x = self.x
        transform.transform.translation.y = self.y
        transform.transform.translation.z = 0.0
        transform.transform.rotation.x = 0.0
        transform.transform.rotation.y = 0.0
        transform.transform.rotation.z = sin(self.theta)
        transform.transform.rotation.w = cos(self.theta)
        broadcaster.sendTransform(transform)
        
        # Calculate v and w from wheel angular velocities.
        wr = (msg.velocityr/100.0)   
        wl = (msg.velocityl/100.0)
        v = self.rw_2*(wr + wl)*6.2832      # Multiply with 2*pi to convert to rad/s
        w = self.rw_dw*(wr - wl)*6.2832
        if w == 0.0:
            R = 10000.0
        else:
            R = v/w
  
            
        # Calculate change in pose.
        # Below code is just a test. We should use dt as measured from the RC and not from
        # timestamp in ROS.
        dt = ((timestamp - self.lasttime).nanoseconds)/1000000000 # The clock timestamp value is in ns. 
                                                                  # We need to convert to second.
        #self.get_logger().info("dt in sec " + str(dt))
    
        dtheta = w * dt
        theta_new = self.theta + dtheta
        dx = -R*sin(self.theta) + R*sin(theta_new)
        dy = R*cos(self.theta) - R*cos(theta_new)
        self.x = self.x + dx
        self.y = self.y + dy
        self.theta = theta_new
        self.lasttime = timestamp
   
        # Debug messages
        #self.get_logger().info("wr " + str(wr))
        #self.get_logger().info("wl " + str(wl))
        self.get_logger().info("v " + str(v))
        self.get_logger().info("w " + str(w))  
        self.get_logger().info("th " + str(self.theta)) 
        self.get_logger().info("x " + str(self.x))
        self.get_logger().info("y " + str(self.y)) 

        
        # Set the pose, in (x,y,z) and quarternion (x,y,z,w)
        # In the quarternion the unit vector for rotation axis is
        # [kx ky kz]^T = [0 0 1]^T
        # and theta = theta_new
        self.odom_msg.pose.pose.position.x = self.x
        self.odom_msg.pose.pose.position.y = self.y
        self.odom_msg.pose.pose.position.z = 0.0
        self.odom_msg.pose.pose.orientation.x = 0.0
        self.odom_msg.pose.pose.orientation.y = 0.0
        self.odom_msg.pose.pose.orientation.z = sin(theta_new)
        self.odom_msg.pose.pose.orientation.w = cos(theta_new)

        # Set the twist.
        self.odom_msg.twist.twist.linear.x = v*cos(theta_new)
        self.odom_msg.twist.twist.linear.y = v*sin(theta_new)
        self.odom_msg.twist.twist.linear.z = 0.0
        self.odom_msg.twist.twist.angular.z = w

        self.publisher_r.publish(self.ranger_msg)       # Publish right range sensor message.
        self.publisher_l.publish(self.rangel_msg)       # Publish left range sensor message.
        self.publisher_t.publish(self.rangef_msg)       # Publish front range message.

        self.publisher_odom.publish(self.odom_msg)      # Publish odometry message.

def main(args=None):
    rclpy.init(args=args)   # Initialize ROS communication library.
    node = RCStatePubNode()  # Instantiate class.
    rclpy.spin(node)        # Block the current node so that other process can run.
    rclpy.shutdown()        # Shutdown.

if __name__ == "__main__":  # Only run this part if this file (or module as python file
                            # is called) is invoked directly by the python intepreter.
    main()