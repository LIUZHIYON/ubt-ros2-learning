#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"

using namespace std::chrono_literals;

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("basic_shapes");
  auto marker_pub = node->create_publisher<visualization_msgs::msg::Marker>(
    "visualization_marker", 10);

  rclcpp::WallRate loop_rate(1s);

  // 从 CUBE 开始，每循环一次切换形状：CUBE → SPHERE → ARROW → CYLINDER → CUBE ...
  uint32_t shape = visualization_msgs::msg::Marker::CUBE;

  while (rclcpp::ok()) {
    visualization_msgs::msg::Marker marker;

    // ========== 头部 ==========
    // frame_id: 标记所在的坐标系（RViz 的 Fixed Frame 要设为这个）
    marker.header.frame_id = "my_frame";
    marker.header.stamp = node->now();

    // ========== 标识 ==========
    // namespace + id 唯一标识一个标记，新的同 ns+id 消息会替换旧的
    marker.ns = "basic_shapes";
    marker.id = 0;

    // ========== 类型 ==========
    marker.type = shape;

    // ========== 动作 ==========
    // ADD = 创建或修改（没删就一直在）
    // DELETE = 删除指定 ns+id 的标记
    // DELETEALL = 删除所有标记
    marker.action = visualization_msgs::msg::Marker::ADD;

    // ========== 位置和姿态 ==========
    marker.pose.position.x = 0.0;
    marker.pose.position.y = 0.0;
    marker.pose.position.z = 0.0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // ========== 尺寸（1.0 = 1 米）==========
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    // ========== 颜色（RGBA, 0~1）==========
    // a 不设为 1 的话默认是透明的！这是最常见的坑
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    // ========== 生命周期 ==========
    // 0 = 永不过期，单位：纳秒
    marker.lifetime = rclcpp::Duration::from_nanoseconds(0);

    // 发布
    marker_pub->publish(marker);
    RCLCPP_INFO(node->get_logger(), "Publishing shape type: %d", shape);

    // 切换到下一个形状
    switch (shape) {
      case visualization_msgs::msg::Marker::CUBE:
        shape = visualization_msgs::msg::Marker::SPHERE;
        break;
      case visualization_msgs::msg::Marker::SPHERE:
        shape = visualization_msgs::msg::Marker::ARROW;
        break;
      case visualization_msgs::msg::Marker::ARROW:
        shape = visualization_msgs::msg::Marker::CYLINDER;
        break;
      case visualization_msgs::msg::Marker::CYLINDER:
        shape = visualization_msgs::msg::Marker::CUBE;
        break;
    }

    loop_rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
