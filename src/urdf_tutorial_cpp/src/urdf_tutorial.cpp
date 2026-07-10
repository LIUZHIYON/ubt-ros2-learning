/**
 * urdf_tutorial.cpp — 机器人关节状态发布器
 *
 * 这个节点不断更新关节角度，发布到 /joint_states，
 * 同时广播 TF 变换让整个机器人在世界里"走路"。
 *
 * 3 个关节的运动模式：
 *  - head_swivel：头部绕 Z 轴无限旋转
 *  - base_to_right_leg：固定关节（不发布）
 *  - base_to_left_leg：固定关节（不发布）
 */

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <cmath>

using namespace std::chrono_literals;

class StatePublisher : public rclcpp::Node
{
public:
  StatePublisher()
  : Node("state_publisher")
  {
    // 发布 /joint_states，robot_state_publisher 订阅这个来计算 TF
    joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);

    // TF 广播器：把机器人的位置（axis 坐标系）发到 odom 坐标系
    tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

    // 定时器：每 20ms（50Hz）调用一次 update()
    timer_ = this->create_wall_timer(20ms, std::bind(&StatePublisher::update, this));

    RCLCPP_INFO(this->get_logger(), "StatePublisher started! R2D2 is walking...");
  }

private:
  void update()
  {
    // 读取当前时间
    auto now = this->get_clock()->now();

    // ====================================================
    // 1. 更新关节角度（模拟机器人运动）
    // ====================================================

    // head_swivel：头部持续旋转（每周期转 1 度）
    swivel_ += degree_;
    if (swivel_ > 2 * M_PI) swivel_ -= 2 * M_PI;

    // tilt 用于模拟"身体倾斜"（这里不直接对应某个关节，纯粹演示效果）
    tilt_ += tilt_inc_;
    if (tilt_ < -0.5 || tilt_ > 0.0) {
      tilt_inc_ *= -1;
    }

    // height 用于模拟"整体上下跳动"
    height_ += height_inc_;
    if (height_ > 0.2 || height_ < 0.0) {
      height_inc_ *= -1;
    }

    // angle 用于让整个机器人在 odom 世界里转圈
    angle_ += degree_;

    // ====================================================
    // 2. 发布 JointState 消息
    // ====================================================
    auto joint_state = sensor_msgs::msg::JointState();
    joint_state.header.stamp = now;
    joint_state.name.push_back("head_swivel");
    joint_state.position.push_back(swivel_);

    joint_pub_->publish(joint_state);

    // ====================================================
    // 3. 广播 TF 变换：odom → axis（机器人在世界里的位置）
    //    让机器人绕圈走动
    // ====================================================
    geometry_msgs::msg::TransformStamped odom_to_axis;
    odom_to_axis.header.stamp = now;
    odom_to_axis.header.frame_id = "odom";       // 世界坐标系
    odom_to_axis.child_frame_id = "axis";         // 机器人底座坐标系

    // 机器人沿圆周运动（角度=angle_，半径=2.0m）
    odom_to_axis.transform.translation.x = cos(angle_) * 2.0;
    odom_to_axis.transform.translation.y = sin(angle_) * 2.0;
    odom_to_axis.transform.translation.z = height_;

    // 绕 Y 轴倾斜 + 绕 Z 轴旋转（面向运动方向）
    tf2::Quaternion q;
    q.setRPY(0.0, tilt_, angle_ + M_PI / 2);
    odom_to_axis.transform.rotation = tf2::toMsg(q);

    tf_broadcaster_->sendTransform(odom_to_axis);

    // 每秒打印一次日志
    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000,
                         "Publishing joint state (head_swivel=%.2f rad)", swivel_);
  }

  // 成员变量
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;

  // 状态变量
  double swivel_ = 0.0;       // 头部旋转角度
  double tilt_ = 0.0;         // 倾斜角
  double tilt_inc_ = 0.05;
  double height_ = 0.0;       // 高度
  double height_inc_ = 0.005;
  double angle_ = 0.0;        // 全局旋转角度
  double degree_ = M_PI / 180; // 1 度 = π/180 弧度
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatePublisher>());
  rclcpp::shutdown();
  return 0;
}
