#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"

// 这个函数接收一个指向 Node 的指针，然后通过它访问各个子接口
void template_interface_demo(const rclcpp::Node::SharedPtr & node)
{
  // -------------------------------------------------------------
  // 1. 通过 Node 的模板方法获取各个接口
  //    get_node_X_interface() 返回对应的子接口指针
  // -------------------------------------------------------------

  // 参数接口
  auto params_iface = node->get_node_parameters_interface();

  // 话题接口
  auto topics_iface = node->get_node_topics_interface();

  // 定时器接口
  auto timers_iface = node->get_node_timers_interface();

  // 日志接口
  auto logging_iface = node->get_node_logging_interface();

  // 时钟接口
  auto clock_iface = node->get_node_clock_interface();

  // -------------------------------------------------------------
  // 2. 通过参数接口设置/读取参数
  //    即使不确定有没有声明过这个参数，也能直接操作
  // -------------------------------------------------------------

  RCLCPP_INFO(logging_iface->get_logger(), "=== 参数接口演示 ===");

  // Humble 要求先声明（或用 allow_undeclared）
  node->declare_parameter<int>("my_int", 0);
  node->declare_parameter<std::string>("my_string", "");
  node->declare_parameter<double>("my_double", 0.0);

  // 设置参数
  auto set_result = params_iface->set_parameters({
    rclcpp::Parameter("my_int", 42),
    rclcpp::Parameter("my_string", "hello"),
    rclcpp::Parameter("my_double", 3.14),
  });

  for (auto & result : set_result) {
    RCLCPP_INFO(logging_iface->get_logger(), "  设置结果: %s",
      result.successful ? "成功" : "失败");
  }

  // 获取参数
  auto param = params_iface->get_parameter("my_int");
  RCLCPP_INFO(logging_iface->get_logger(), "  my_int = %ld", param.as_int());

  // -------------------------------------------------------------
  // 3. 通过话题接口查询
  // -------------------------------------------------------------

  RCLCPP_INFO(logging_iface->get_logger(), "=== 话题接口演示 ===");

  // 获取话题信息（Humble 中通过 graph 接口查询）
  auto graph_iface = node->get_node_graph_interface();
  auto pub_topics = graph_iface->get_publisher_names_and_types_by_node(
    node->get_name(), node->get_namespace());
  RCLCPP_INFO(logging_iface->get_logger(), "  已有发布者: %zu 个", pub_topics.size());

  // -------------------------------------------------------------
  // 4. 通过定时器接口
  // -------------------------------------------------------------

  RCLCPP_INFO(logging_iface->get_logger(), "=== 定时器接口演示 ===");

  // 创建一个定时器（Humble 中通过 Node 直接创建）
  auto timer = node->create_wall_timer(
    std::chrono::seconds(1),
    [logging_iface]() {
      RCLCPP_INFO(logging_iface->get_logger(), "  定时器触发了！");
    });

  RCLCPP_INFO(logging_iface->get_logger(), "  定时器已创建");

  // -------------------------------------------------------------
  // 5. 通过时钟接口
  // -------------------------------------------------------------

  RCLCPP_INFO(logging_iface->get_logger(), "=== 时钟接口演示 ===");

  auto now = clock_iface->get_clock()->now();
  RCLCPP_INFO(logging_iface->get_logger(), "  当前时间: %.3f 秒",
    now.seconds());
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  // 创建节点
  auto node = std::make_shared<rclcpp::Node>("node_interfaces_demo");

  RCLCPP_INFO(node->get_logger(), "节点已创建，名称: %s", node->get_name());

  // 调用演示函数
  template_interface_demo(node);

  // 让节点跑一下，让定时器有机会触发
  rclcpp::spin_some(node);

  rclcpp::shutdown();
  return 0;
}
