/**
 * rosbag2 C++ 读 Bag 演示
 *
 * 功能：
 *   1. 用 rosbag2_cpp::Reader 读取之前录制的 bag 文件
 *   2. 逐条打印出消息内容
 *   3. 同时把消息重新发布出去（可选）
 *
 * 运行：
 *   ros2 run bag_reader_demo bag_reader
 *
 * 先确保有 bag 文件（用 bag_recorder_demo 录一个）：
 *   ros2 run bag_recorder_demo bag_recorder
 */

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "rosbag2_cpp/reader.hpp"
#include "rosbag2_cpp/readers/sequential_reader.hpp"
#include "rosbag2_storage/storage_options.hpp"

class BagReaderNode : public rclcpp::Node
{
public:
  BagReaderNode()
  : Node("bag_reader_node")
  {
    // 同时创建一个 publisher，读完 bag 后把消息发出去
    publisher_ = this->create_publisher<std_msgs::msg::String>("bag_replay_topic", 10);
  }

  void read_bag(const std::string & uri)
  {
    // ---- 1. 创建 Reader ----
    rosbag2_cpp::Reader reader(
      std::make_unique<rosbag2_cpp::readers::SequentialReader>()
    );

    // ---- 2. 打开 bag ----
    reader.open(uri);

    RCLCPP_INFO(this->get_logger(), "📖 开始读取 bag: %s", uri.c_str());

    // ---- 3. 获取 bag 元信息 ----
    auto metadata = reader.get_metadata();
    RCLCPP_INFO(this->get_logger(), "  包含 %zu 个 topic", metadata.topics_with_message_count.size());
    for (const auto & t : metadata.topics_with_message_count) {
      RCLCPP_INFO(this->get_logger(),
        "    Topic: %s | Type: %s | Count: %zu",
        t.topic_metadata.name.c_str(),
        t.topic_metadata.type.c_str(),
        t.message_count);
    }

    // ---- 4. 逐条读取消息 ----
    size_t count = 0;
    while (reader.has_next()) {
      // 模板方法自动反序列化
      auto msg = reader.read_next<std_msgs::msg::String>();

      RCLCPP_INFO(this->get_logger(),
        "[%zu] data='%s'",
        count,
        msg.data.c_str());

      // 重新发布出去
      publisher_->publish(msg);
      count++;
    }

    RCLCPP_INFO(this->get_logger(), "📖 读取完毕，共 %zu 条消息", count);
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  printf("\n"
    "========================================\n"
    "  rosbag2 C++ 读 Bag 演示\n"
    "========================================\n"
    "  读取 my_bag/ 中的所有消息\n"
    "  打印内容并重新发布到 /bag_replay_topic\n"
    "========================================\n\n");

  auto node = std::make_shared<BagReaderNode>();
  node->read_bag("my_bag");
  rclcpp::shutdown();
  return 0;
}
