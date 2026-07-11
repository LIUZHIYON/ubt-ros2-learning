/**
 * rosbag2 C++ API 演示 —— 从节点内部录制话题（Humble 兼容版）
 *
 * 功能：
 *   1. 创建 publisher 发布消息
 *   2. 同时用 rosbag2_cpp::Writer 写到 .mcap bag 文件
 *   3. Ctrl-C 停止，bag 自动保存
 *
 * 运行：         ros2 run bag_recorder_demo bag_recorder
 * 回放：         ros2 bag play my_bag/
 * 查看 bag 信息： ros2 bag info my_bag/
 */

#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "rosbag2_cpp/writer.hpp"
#include "rosbag2_cpp/writers/sequential_writer.hpp"
#include "rosbag2_storage/storage_options.hpp"

using namespace std::chrono_literals;

class BagRecorderNode : public rclcpp::Node
{
public:
  BagRecorderNode()
  : Node("bag_recorder_node"), count_(0)
  {
    // ---- 1. 创建 publisher ----
    publisher_ = this->create_publisher<std_msgs::msg::String>("bag_demo_topic", 10);

    // ---- 2. 配置 rosbag2 Writer ----
    writer_ = std::make_unique<rosbag2_cpp::Writer>(
      std::make_unique<rosbag2_cpp::writers::SequentialWriter>()
    );

    // 存储选项
    rosbag2_storage::StorageOptions storage_options;
    storage_options.uri = "my_bag";
    storage_options.storage_id = "sqlite3";    // ROS2 Humble 默认格式

    // 打开 writer（自动录制所有 topic）
    writer_->open(storage_options);
    RCLCPP_INFO(this->get_logger(), "📦 开始录制 -> %s", storage_options.uri.c_str());

    // ---- 3. 定时器：每 500ms 发布并录制 ----
    timer_ = this->create_wall_timer(
      500ms,
      std::bind(&BagRecorderNode::timer_callback, this)
    );
  }

  ~BagRecorderNode()
  {
    RCLCPP_INFO(this->get_logger(), "📦 录制停止，共 %lu 条消息", count_);
  }

private:
  void timer_callback()
  {
    auto msg = std::make_shared<std_msgs::msg::String>();
    msg->data = "Hello from bag recorder #" + std::to_string(count_);

    RCLCPP_INFO(this->get_logger(), "Recording: '%s'", msg->data.c_str());

    // ---- 4. 写入 bag（模板方法自动序列化） ----
    writer_->write(*msg, "bag_demo_topic", this->now());

    publisher_->publish(*msg);
    count_++;
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::unique_ptr<rosbag2_cpp::Writer> writer_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  printf("\n"
    "========================================\n"
    "  rosbag2 C++ API 录制演示\n"
    "========================================\n"
    "  每 500ms 发布并录制到 my_bag/\n"
    "  Ctrl-C 停止，bag 自动保存\n"
    "========================================\n\n");

  auto node = std::make_shared<BagRecorderNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
