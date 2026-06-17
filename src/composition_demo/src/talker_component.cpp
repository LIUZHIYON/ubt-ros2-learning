#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>   // ★ 组件注册宏
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

namespace composition_demo
{

class Talker : public rclcpp::Node
{
public:
    // ★ 关键：构造函数必须接收 NodeOptions 参数
    explicit Talker(const rclcpp::NodeOptions & options)
    : Node("talker", options)   // ← options 传给父类
    {
        // 创建发布者
        publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

        // 创建定时器，每 500ms 发一条消息
        timer_ = this->create_wall_timer(
            500ms,
            std::bind(&Talker::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Talker 组件已启动！");
    }

private:
    void timer_callback()
    {
        auto msg = std_msgs::msg::String();
        msg.data = "Hello World " + std::to_string(count_++);
        RCLCPP_INFO(this->get_logger(), "发布: '%s'", msg.data.c_str());
        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_ = 0;
};

}  // namespace composition_demo

// ★★★ 最重要的一行：注册组件！没有它 ros2 component load 找不到你的类
RCLCPP_COMPONENTS_REGISTER_NODE(composition_demo::Talker)
