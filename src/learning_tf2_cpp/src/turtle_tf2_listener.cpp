#include <chrono>
#include <functional>
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

    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    // 向 turtle2 发布速度指令
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle2/cmd_vel", 10);

    // 先尝试 spawn turtle2
    spawn_turtle2();
  }

private:
  void spawn_turtle2()
  {
    // 每隔 200ms 检查一次 spawn 服务是否就绪
    spawn_check_timer_ = this->create_wall_timer(
      200ms,
      [this]() {
        // 检查服务是否可用
        auto client = this->create_client<turtlesim::srv::Spawn>("/spawn");
        if (!client->wait_for_service(0s)) {
          RCLCPP_INFO_ONCE(this->get_logger(),
            "Waiting for /spawn service... (start turtlesim_node first)");
          return;
        }

        // 服务就绪，发送 spawn 请求
        auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
        request->x = 5.5;
        request->y = 5.5;
        request->theta = 0.0;
        request->name = "turtle2";

        RCLCPP_INFO(this->get_logger(), "Spawning turtle2...");

        client->async_send_request(request,
          [this](rclcpp::Client<turtlesim::srv::Spawn>::SharedFuture future) {
            try {
              auto result = future.get();
              RCLCPP_INFO(this->get_logger(),
                "turtle2 spawned successfully! name='%s'",
                result->name.c_str());
              // turtle2 创建成功，开始广播它的 tf
              start_broadcasting_turtle2();
              stop_spawn_check_timer();
            } catch (const std::exception & e) {
              RCLCPP_ERROR(this->get_logger(),
                "Failed to spawn turtle2: %s", e.what());
            }
          });
      });
  }

  void stop_spawn_check_timer()
  {
    if (spawn_check_timer_) {
      spawn_check_timer_->cancel();
    }
  }

  void start_broadcasting_turtle2()
  {
    // 创建广播器
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    // 订阅 turtle2 的 pose
    pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
      "/turtle2/pose", 10,
      [this](const turtlesim::msg::Pose::SharedPtr msg) {
        last_pose_ = msg;

        // 广播 turtle2 的变换
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

    // 启动追踪定时器（pose 到了就开始追）
    timer_ = this->create_wall_timer(
      100ms,
      std::bind(&FrameListener::on_timer, this));

    RCLCPP_INFO(this->get_logger(), "Started broadcasting tf for turtle2");
  }

  void on_timer()
  {
    if (!last_pose_) {
      return;  // 还没收到 turtle2 的 pose，不查变换
    }

    std::string target_frame = this->get_parameter("target_frame").as_string();

    geometry_msgs::msg::TransformStamped t;
    try {
      t = tf_buffer_->lookupTransform(
        "turtle2",
        target_frame,
        tf2::TimePointZero);
    } catch (const tf2::TransformException & ex) {
      RCLCPP_INFO_ONCE(
        this->get_logger(), "Could not transform %s to turtle2: %s",
        target_frame.c_str(), ex.what());
      return;
    }

    geometry_msgs::msg::Twist msg;
    static const double kAngular = 1.0;
    static const double kLinear = 0.5;

    msg.linear.x = kLinear * sqrt(
      t.transform.translation.x * t.transform.translation.x +
      t.transform.translation.y * t.transform.translation.y);
    msg.angular.z = kAngular * atan2(
      t.transform.translation.y,
      t.transform.translation.x);

    publisher_->publish(msg);
  }

  // --- 成员变量 ---
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr spawn_check_timer_;
  turtlesim::msg::Pose::SharedPtr last_pose_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FrameListener>());
  rclcpp::shutdown();
  return 0;
}
