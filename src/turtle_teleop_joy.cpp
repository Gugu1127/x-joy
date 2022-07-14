#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

// create the TeleopTurtle class and define the joyCallback function that will take a joy msg
class TeleopTurtle {
   public:
    TeleopTurtle() {
        //  initialize some parameters
        linear_ = 1;
        angular_ = 2;
        nodePtr.param("axis_linear", linear_, linear_);
        nodePtr.param("axis_angular", angular_, angular_);
        nodePtr.param("scale_angular", a_scale_, a_scale_);
        nodePtr.param("scale_linear", l_scale_, l_scale_);

        // create a publisher that will advertise on the command_velocity topic of the turtle
        vel_pub_ = nodePtr.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);

        // subscribe to the joystick topic for the input to drive the turtle
        joy_sub_ = nodePtr.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);
    }

   private:
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {
        geometry_msgs::Twist twist;

        // take the data from the joystick and manipulate it by scaling it and using independent axes to control the linear and angular velocities of the turtle
        twist.angular.z = 5 * joy->axes[angular_];
        float vol_forward = -joy->axes[RT] + 1;
        float vol_back = -joy->axes[LT] + 1;
        if (vol_back > 0) {
          twist.linear.x = -5 * vol_back;
        }
        if (vol_forward > 0) {
          twist.linear.x = 5 * vol_forward;
        }

        vel_pub_.publish(twist);
    }

    ros::NodeHandle nodePtr;

    int LT = 2, RT = 5;
    int linear_, angular_;  // used to define which axes of the joystick will control our turtle
    double l_scale_, a_scale_;
    ros::Publisher vel_pub_;
    ros::Subscriber joy_sub_;
};

int main(int argc, char** argv) {
    // initialize our ROS node, create a teleop_turtle, and spin our node until Ctrl-C is pressed
    ros::init(argc, argv, "teleop_turtle");
    TeleopTurtle teleop_turtle;

    ros::spin();
}
