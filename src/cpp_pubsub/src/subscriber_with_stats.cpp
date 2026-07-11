/**
 * 带 Topic Statistics 的 subscriber
 * 
 * 对比原始 subscriber.cpp 的变化：
 * 1. 多了 #include <chrono> 和 #include "rclcpp/subscription_options.hpp"
 * 2. 创建订阅前配置 SubscriptionOptions 开启统计
 * 3. 统计结果默认发布到 /statistics 话题
 * 
 * 查看统计：ros2 topic echo /statistics
 */

#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/subscription_options.hpp"          // ← 新增：订阅选项头文件
#include "tutorial_interfaces/msg/num.hpp"

using std::placeholders::_1;

class MinimalSubscriberWithStats : public rclcpp::Node {
public:
  MinimalSubscriberWithStats()
  : Node("minimal_subscriber_with_stats")           // ← 改了个不同的节点名
  {
    // ---- 关键：配置 Topic Statistics ----
    auto options = rclcpp::SubscriptionOptions();
    options.topic_stats_options.state =
      rclcpp::TopicStatisticsState::Enable;          // ← 开启统计

    // 统计报告的发布周期（默认 1 秒），这里设为 10 秒
    options.topic_stats_options.publish_period =
      std::chrono::seconds(10);

    // （可选）自定义统计输出的话题名，默认是 /statistics
    // options.topic_stats_options.publish_topic = "/my_statistics";

    // 注意：create_subscription 多传了 options 参数
    subscription_ = this->create_subscription<tutorial_interfaces::msg::Num>(
      "topic", 10,
      std::bind(&MinimalSubscriberWithStats::topic_callback, this, _1),
      options                                       // ← 第 4 个参数
    );
  }

private:
  void topic_callback(const tutorial_interfaces::msg::Num &msg) const
  {
    RCLCPP_INFO(this->get_logger(), "I heard: '%ld'", msg.num);
  }

  rclcpp::Subscription<tutorial_interfaces::msg::Num>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriberWithStats>());
  rclcpp::shutdown();
  return 0;
}
