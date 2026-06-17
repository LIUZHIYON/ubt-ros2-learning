#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/parameter_event.hpp"

class SampleNodeWithParameters : public rclcpp::Node
{
public:
  SampleNodeWithParameters()
  : Node("node_with_parameters")
  {
    // 声明两个参数
    this->declare_parameter("an_int_param", 0);
    this->declare_parameter("another_double_param", 0.0);

    // 创建参数事件处理器
    param_subscriber_ =
      std::make_shared<rclcpp::ParameterEventHandler>(this);

    // ── 单个参数回调：自己的 an_int_param ──
    auto cb = [this](const rclcpp::Parameter & p) {
      RCLCPP_INFO(get_logger(),
        "[cb] 参数 \"%s\" -> %ld", p.get_name().c_str(), p.as_int());
    };
    cb_handle_ = param_subscriber_->add_parameter_callback("an_int_param", cb);

    // ── 远程参数回调：监控 parameter_blackboard 的参数 ──
    auto cb2 = [this](const rclcpp::Parameter & p) {
      RCLCPP_INFO(get_logger(),
        "[cb2] 远程参数 \"%s\" -> %.02lf", p.get_name().c_str(), p.as_double());
    };
    cb_handle2_ = param_subscriber_->add_parameter_callback(
      "a_double_param", cb2, "parameter_blackboard");

    // ── 全局事件回调：所有参数变动都触发 ──
    auto event_cb = [this](const rcl_interfaces::msg::ParameterEvent & evt) {
      RCLCPP_INFO(get_logger(),
        "[event] 来自节点 \"%s\"", evt.node.c_str());
      for (const auto & p : evt.changed_parameters) {
        RCLCPP_INFO(get_logger(),
          "  \"%s\" -> %s",
          p.name.c_str(),
          rclcpp::Parameter::from_parameter_msg(p).value_to_string().c_str());
      }
    };
    event_cb_handle_ = param_subscriber_->add_parameter_event_callback(event_cb);
  }

private:
  std::shared_ptr<rclcpp::ParameterEventHandler> param_subscriber_;
  std::shared_ptr<rclcpp::ParameterCallbackHandle> cb_handle_;
  std::shared_ptr<rclcpp::ParameterCallbackHandle> cb_handle2_;
  std::shared_ptr<rclcpp::ParameterEventCallbackHandle> event_cb_handle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SampleNodeWithParameters>());
  rclcpp::shutdown();
  return 0;
}
