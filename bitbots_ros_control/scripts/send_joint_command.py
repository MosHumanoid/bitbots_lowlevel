#!/usr/bin/env python3

import rospy
from bitbots_msgs.msg import JointCommand
import math

"""ids = {"HeadPan": 0,
               "HeadTilt": 1,
               "LShoulderPitch": 2,
               "LShoulderRoll": 3,
               "LElbow": 4,
               "RShoulderPitch": 5,
               "RShoulderRoll": 6,
               "RElbow": 7,
               "LHipYaw": 8,
               "LHipRoll": 9,
               "LHipPitch": 10,
               "LKnee": 11,
               "LAnklePitch": 12,
               "LAnkleRoll": 13,
               "RHipYaw": 14,
               "RHipRoll": 15,
               "RHipPitch": 16,
               "RKnee": 17,
               "RAnklePitch": 18,
               "RAnkleRoll": 19}
"""

ids = { "RShoulderPitch": 0}
rospy.init_node("send_joint_command")

pos_msg = JointCommand()
pos_msg.joint_names = ids.keys()
#pos_msg.joint_names = ["RShoulderRoll"]
pos_msg.velocities = [-1] * len(ids)
pos_msg.positions = [0] *  len(ids)
pos_msg.accelerations = [-1] *  len(ids)
pos_msg.max_currents = [-1.0] *  len(ids)

pub = rospy.Publisher("/DynamixelController/command", JointCommand, queue_size=1)
print(pos_msg)
rate = rospy.Rate(100)
while not rospy.is_shutdown():
    pos_msg.header.stamp = rospy.Time.now()
    pos_msg.positions = [math.sin(rospy.get_time() % (2 * math.pi)) * math.pi ]
    pub.publish(pos_msg)  
    rate.sleep()
    