#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <std_msgs/msg/string.hpp>

namespace composition_demo
{

class Listener : public rclcpp::Node
{
public:
    explicit Listener(const rclcpp::NodeOptions & options)
    : Node("listener", options)
    {
        // 订阅 chatter 话题（和 Talker 同一个话题）
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "chatter", 10,
            std::bind(&Listener::topic_callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Listener 组件已启动！等待消息...");
    }

private:
    void topic_callback(const std_msgs::msg::String & msg)
    {
        RCLCPP_INFO(this->get_logger(), "收到: '%s'", msg.data.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

}  // namespace composition_demo

RCLCPP_COMPONENTS_REGISTER_NODE(composition_demo::Listener)
