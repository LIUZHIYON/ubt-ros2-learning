#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/msg/pose.hpp"

class FramePublisher : public rclcpp::Node
{
public:
  FramePublisher()
  : Node("turtle_tf2_broadcaster")
  {
    // 创建 TransformBroadcaster
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    // 声明一个参数 "turtlename"，默认值是 "turtle1"
    this->declare_parameter<std::string>("turtlename", "turtle1");

    // 获取参数
    std::string turtlename = this->get_parameter("turtlename").as_string();

    // 订阅对应乌龟的 pose 话题
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      "/" + turtlename + "/pose", 10,
      [this, turtlename](const turtlesim::msg::Pose::SharedPtr msg) {
        handle_turtle_pose(msg, turtlename);
      });
  }

private:
  void handle_turtle_pose(
    const turtlesim::msg::Pose::SharedPtr msg,
    const std::string & turtlename)
  {
    // 构造 TransformStamped 消息
    geometry_msgs::msg::TransformStamped t;

    // 时间戳用现在
    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "world";
    t.child_frame_id = turtlename;  // 比如 "turtle1"

    // 乌龟的 x, y 位置
    t.transform.translation.x = msg->x;
    t.transform.translation.y = msg->y;
    t.transform.translation.z = 0.0;

    // 乌龟朝向：从 yaw（偏航角）转成四元数
    tf2::Quaternion q;
    q.setRPY(0, 0, msg->theta);  // roll=0, pitch=0, yaw=theta
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    // 发送变换
    tf_broadcaster_->sendTransform(t);
  }

  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FramePublisher>());
  rclcpp::shutdown();
  return 0;
}
