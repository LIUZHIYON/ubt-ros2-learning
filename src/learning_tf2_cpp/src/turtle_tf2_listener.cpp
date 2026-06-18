#include <chrono>
#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/buffer.h"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

class FrameListener : public rclcpp::Node
{
public:
  FrameListener()
  : Node("turtle_tf2_frame_listener")
  {
    this->declare_parameter<std::string>("target_frame", "turtle1");

    // --- 1. 创建 tf2 基础设施 ---
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    // --- 2. spawn turtle2（同步等待，确保创建完毕再继续）---
    spawn_turtle2();

    // --- 3. 订阅 turtle2 的 pose → 广播 tf 变换 ---
    pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
      "/turtle2/pose", 10,
      [this](const turtlesim::msg::Pose::SharedPtr msg) {
        last_pose_ = msg;

        geometry_msgs::msg::TransformStamped t;
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "world";
        t.child_frame_id = "turtle2";
        t.transform.translation.x = msg->x;
        t.transform.translation.y = msg->y;
        t.transform.translation.z = 0.0;

        tf2::Quaternion q;
        q.setRPY(0, 0, msg->theta);
        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();

        tf_broadcaster_->sendTransform(t);
      });

    // --- 4. 发布速度指令让 turtle2 追 turtle1 ---
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle2/cmd_vel", 10);

    // --- 5. 定时控制循环（100ms）---
    timer_ = this->create_wall_timer(
      100ms, [this]() { on_timer(); });
  }

private:
  void spawn_turtle2()
  {
    auto client = this->create_client<turtlesim::srv::Spawn>("/spawn");

    // 等待服务就绪
    RCLCPP_INFO(this->get_logger(), "Waiting for /spawn service...");
    if (!client->wait_for_service(5s)) {
      RCLCPP_ERROR(this->get_logger(),
        "/spawn service not available! Is turtlesim_node running?");
      return;
    }

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->x = 5.5;
    request->y = 5.5;
    request->theta = 0.0;
    request->name = "turtle2";

    RCLCPP_INFO(this->get_logger(), "Spawning turtle2 at (5.5, 5.5)...");

    // 同步等待 spawn 结果
    auto future = client->async_send_request(request);
    auto status = rclcpp::spin_until_future_complete(
      this->get_node_base_interface(), future, 3s);

    if (status == rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_INFO(this->get_logger(), "✓ turtle2 spawned successfully!");
    } else {
      RCLCPP_ERROR(this->get_logger(), "Failed to spawn turtle2 (timeout or error)");
    }
  }

  void on_timer()
  {
    // 还没收到 turtle2 的 pose → 跳过
    if (!last_pose_) {
      RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 3000,
        "Waiting for /turtle2/pose...");
      return;
    }

    std::string target_frame = this->get_parameter("target_frame").as_string();

    geometry_msgs::msg::TransformStamped t;
    try {
      // 查询 target_frame(turtle1) 相对于 turtle2 的位置
      t = tf_buffer_->lookupTransform(
        "turtle2", target_frame, this->get_clock()->now());
    } catch (const tf2::TransformException & ex) {
      RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 3000,
        "Waiting for transform %s → turtle2: %s",
        target_frame.c_str(), ex.what());
      return;
    }

    // 简单 P 控制器：turtle2 朝 turtle1 前进
    geometry_msgs::msg::Twist msg;
    static const double kAngular = 1.0;
    static const double kLinear = 0.5;

    double dx = t.transform.translation.x;
    double dy = t.transform.translation.y;

    msg.linear.x = kLinear * sqrt(dx * dx + dy * dy);
    msg.angular.z = kAngular * atan2(dy, dx);

    publisher_->publish(msg);
  }

  // --- 成员变量 ---
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
  turtlesim::msg::Pose::SharedPtr last_pose_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FrameListener>());
  rclcpp::shutdown();
  return 0;
}
