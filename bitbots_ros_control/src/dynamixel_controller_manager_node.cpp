#include <bitbots_ros_control/dynamixel_hardware_interface.h>
#include <ros/callback_queue.h>
#include <controller_manager/controller_manager.h>
#include <dynamic_reconfigure/server.h>
#include <bitbots_ros_control/bitbots_ros_control_paramsConfig.h>


int main(int argc, char** argv)
{
  ros::init(argc, argv, "dynamixel_controller_manager");
  ros::NodeHandle pnh("~");

  // Load dynamixels
  bitbots_ros_control::DynamixelHardwareInterface hw;


  // set the dynamic reconfigure and load standard params
  dynamic_reconfigure::Server<bitbots_ros_control::bitbots_ros_control_paramsConfig> server;
  dynamic_reconfigure::Server<bitbots_ros_control::bitbots_ros_control_paramsConfig>::CallbackType f;
  f = boost::bind(&bitbots_ros_control::DynamixelHardwareInterface::reconf_callback,&hw, _1, _2);
  server.setCallback(f);

  if (!hw.init(pnh))
  {
    ROS_ERROR_STREAM("Failed to initialize hardware interface.");
    return 1;
  }

  bool controller_stopped = false;

  // Create separate queue, because otherwise controller manager will freeze
  ros::NodeHandle nh;
  ros::CallbackQueue queue;
  nh.setCallbackQueue(&queue);
  ros::AsyncSpinner spinner(1, &queue);
  spinner.start();
  controller_manager::ControllerManager cm(&hw, nh);

  // Start control loop
  ros::Time current_time = ros::Time::now();
  bool first_update = true;
  ros::Rate rate(pnh.param("control_loop_hz", 200));

  // Remember how many times we failed to read
  int num_failed_reads = 0;

  while (ros::ok())
  {
    bool read_sucessfull = hw.read();
    ros::Duration period = ros::Time::now() - current_time;
    current_time = ros::Time::now();
    if(read_sucessfull){
      // only write something to hardware
      if (first_update) {
        first_update = false;
      } else {
        // start or stop joint state controller if connection lost / restored
        /*if(read_sucessfull && controller_stopped){
          ROS_WARN("start");
          cm.getControllerByName("joint_state_controller")->startRequest(current_time);
          if(cm.getControllerByName("joint_state_controller")->isRunning()){
            controller_stopped = false;
          }
        }else if (!read_sucessfull && !controller_stopped){
          ROS_WARN("stop");
          cm.getControllerByName("joint_state_controller")->stopRequest(current_time);
          controller_stopped = true;
        }*/
        cm.update(current_time, period);
      }
      hw.write();
    } else {
      num_failed_reads++;
      if (num_failed_reads >= 5000) {
        ROS_ERROR("Reinitilizing hardware interface");
        hw.init(pnh);
        num_failed_reads = 0;
      }
    }
    rate.sleep();
    ros::spinOnce();
  }
  return 0;
}
