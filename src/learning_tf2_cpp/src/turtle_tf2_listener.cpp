#include <chrono>
#include <functional>
#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "turtlesim/msg/pose.hpp"

class FrameListener : public rclcpp::Node
{
public:
  FrameListener()
  : Node("turtle_tf2_frame_listener"),
    turtle_spawning_service_ready_(false),
    turtle_spawned_(false)
  {
    // 声明目标乌龟参数
    this->declare_parameter<std::string>("target_frame", "turtle1");

    std::string target_frame;
    this->get_parameter("target_frame", target_frame);

    // 创建 tf2 buffer 和 listener —— 核心！
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    // 发布速度指令给 turtle2
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle2/cmd_vel", 10);

    // 每隔 100ms 执行一次定时回调
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(100),
      std::bind(&FrameListener::on_timer, this));
  }

private:
  void on_timer()
  {
    // 获取目标乌龟的名称
    std::string target_frame = this->get_parameter("target_frame").as_string();

    // 从 tf buffer 里查找变换：从 "turtle2" 到 target_frame (turtle1)
    geometry_msgs::msg::TransformStamped t;

    try {
      // 关键 API: lookup_transform(目标坐标系, 源坐标系, 时间)
      t = tf_buffer_->lookupTransform(
        "turtle2",           // target frame
        target_frame,        // source frame
        tf2::TimePointZero); // 最新可用的时间
    } catch (const tf2::TransformException & ex) {
      // 如果变换还没到，或者两个坐标系不在同一个 tf 树里，会抛异常
      RCLCPP_INFO(
        this->get_logger(), "Could not transform %s to turtle2: %s",
        target_frame.c_str(), ex.what());
      return;
    }

    // 到这里说明变换查询成功
    // 根据变换值计算 turtle2 应该怎么转和怎么走
    geometry_msgs::msg::Twist msg;

    // 线速度：跟平移距离成正比（简单 P 控制器）
    static const double scale_rotation_rate = 1.0;
    static const double scale_forward_rate = 0.5;

    msg.linear.x = scale_forward_rate * sqrt(
      t.transform.translation.x * t.transform.translation.x +
      t.transform.translation.y * t.transform.translation.y);

    // 角速度：朝 turtle1 方向转
    msg.angular.z = scale_rotation_rate * atan2(
      t.transform.translation.y,
      t.transform.translation.x);

    // 发布速度指令
    publisher_->publish(msg);
  }

  // --- 成员变量 ---
  bool turtle_spawning_service_ready_;
  bool turtle_spawned_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FrameListener>());
  rclcpp::shutdown();
  return 0;
}
