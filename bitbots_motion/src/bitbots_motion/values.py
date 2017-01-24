import rospy

from .fall_checker import FallChecker
from bitbots_common.utilCython.pydatavector import PyIntDataVector as IntDataVector
from bitbots_common.utilCython.pydatavector import PyDataVector as DataVector

import bitbots_animation.msg
import time
import actionlib


class Values(object):
    """
    We use this class as a singleton to share these public variables over all
    different classes of states, the state machine and the motion node.
    """

    def __init__(self):
        self.penalized = False  # paused
        self.record = False  # record UI running
        self.shut_down = False  # do we want to shut down

        self.standup_flag = False  # stand up disabled
        self.soft_off_flag = True  # soft of enablbed
        self.soft_start = False  # starting in softoff
        self.die_flag = False  # stop the motion after some time without command
        self.start_test = False  # run motor ping on startup

        self.last_hardware_update = None  # time of last update from hardware
        self.last_request = None  # last request on doing something
        self.start_up_time = time.time()

        self.raw_gyro = IntDataVector(0, 0, 0)
        self.smooth_gyro = IntDataVector(0, 0, 0)
        self.not_so_smooth_gyro = IntDataVector(0, 0, 0)
        self.robo_angle = DataVector(0, 0, 0)

        self.fall_checker = FallChecker()
        # for internal animations
        self.animation_client = None
        self.speak_publisher = None

        # we want to play an animation, try to become controllable
        self.external_animation_requested = False
        # play now the external animation, go to animation running
        self.external_animation_play = False
        # the animation is finished, go back to controllable
        self.external_animation_finished = False

        # are we walking?
        self.walking_active = False

        self.softoff_time = rospy.get_param("/motion/soft_off_time")
        self.die_time = rospy.get_param("/motion/die_time")

    def is_falling(self):
        falling_pose = self.fall_checker.check_falling(self.not_so_smooth_gyro)
        if falling_pose is not None:
            return True
        return False

    def is_fallen(self):
        direction_animation = self.fall_checker.check_fallen(self.raw_gyro, self.smooth_gyro,
                                                             self.robo_angle)
        if direction_animation is not None:
            return True
        return False

    def is_soft_off_time(self):
        return self.soft_off_flag and time.time() - self.last_hardware_update > self.softoff_time

    def is_die_time(self):
        return self.die_flag and time.time() - self.last_hardware_update > self.die_time


VALUES = Values()