from bitbots_connector.connector import AbstractConnector
import numpy

# robot states that are published to the rest of the software
# definition from humanoid_league_msgs/RobotControlState.msg
STATE_CONTROLABLE = 0
STATE_FALLING = 1
STATE_FALLEN = 2
STATE_GETTING_UP = 3
STATE_ANIMATION_RUNNING = 4
STATE_STARTUP = 5
STATE_SHUT_DOWN = 6
STATE_PENALTY = 7
STATE_PENALTY_ANIMATION = 8
STATE_RECORD = 9
STATE_WALKING = 10
STATE_MOTOR_OFF=11
STATE_HCM_OFF=12
STATE_HARDWARE_PROBLEM=13
STATE_PICKED_UP = 14


class HcmConnector(AbstractConnector):
    def __init__(self):
        super(HcmConnector, self).__init__()
        self.hcm = HcmCapsule

class HcmCapsule:
    def __init__(self):
        self.penalized = False

        # Imu
        self.accel = numpy.array([0, 0, 0])
        self.gyro = numpy.array([0, 0, 0])
        self.smooth_accel = numpy.array([0, 0, 0])
        self.smooth_gyro = numpy.array([0, 0, 0])
        self.not_much_smoothed_gyro = numpy.array([0, 0, 0])


        self.current_state = STATE_STARTUP

        self.last_animation_goal_time = 0
        self.external_animation_running = True

        self.last_walking_goal_time = 0

        self.record_active = False

        self.fall_checker = FallChecker()
        self.connector.animation_action_client = None

        ####
        self.speak_publisher = None

        self.penalized = False  # paused
        self.record = False  # record UI running
        self.shut_down = False  # do we want to shut down

        self.stand_up_active = True # should the robot stand up        
        self.motors_on = False  # are the motors on
        self.motors_on_start = False  # should the motors be turned on at start
        self.timed_motor_off_active = False  # deactivate motors after some time without commands

        self.last_motor_update = None  # time of last update from hardware
        self.last_motor_command = None  # last request on doing something
        self.start_up_time = 0 # time at start of HCM

        self.raw_gyro = numpy.array([0, 0, 0])
        self.smooth_gyro = numpy.array([0, 0, 0])
        self.not_so_smooth_gyro = numpy.array([0, 0, 0])
        self.smooth_accel = numpy.array([0, 0, 0])
        self.quaternion  = numpy.array([0,0,0,0.21])

        # for internal animations
        self.animation_client = None
        self.hcm_animation_playing = False
        self.hcm_animation_finished = False

        # we want to play an animation, try to become controllable
        self.external_animation_requested = False
        # playing now the external animation, go to animation running
        self.external_animation_playing = False
        # the animation is finished, go back to controllable
        self.external_animation_finished = False

        # are we walking?
        self.walking_active = False

        # used to stop the walking
        # TODO evaluate if necessary
        self.cmd_vel_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=1)

        self.motor_off_time = rospy.get_param("hcm/motor_off_time")        
        # TODO disable Simulation
        self.simulation_active = rospy.get_param("simulation_active")
        






####

    def is_falling(self):
        falling_pose = self.fall_checker.check_falling(self.not_so_smooth_gyro, self.quaternion)
        if falling_pose is not None:
            return True
        return False

    def get_falling_pose(self):
        return self.fall_checker.check_falling(self.not_so_smooth_gyro, self.quaternion)

    def is_fallen(self):
        return self.fall_checker.check_fallen(self.smooth_accel)

    def is_motor_off_time(self):
        if self.simulation_active:
            return False
        if self.last_hardware_update is not None:
            return self.timed_motor_off_active and rospy.get_time() - self.last_hardware_update > self.motor_off_time
        else:
            return self.timed_motor_off_active and rospy.get_time() - self.start_up_time > self.motor_off_time


    

    def animation_finished(self):
        return BLACKBOARD.hcm_animation_finished