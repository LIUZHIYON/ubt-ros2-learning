#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"

// =========================================================================
// 背景问题：
//   rclcpp::Node 和 rclcpp_lifecycle::LifecycleNode 没有共同父类。
//   所以你不能写一个函数同时接受两种类型的节点。
//
//   ❌ 报错：void foo(rclcpp::Node::SharedPtr) → 不能传 LifecycleNode
//   ✅ 办法：C++ 模板，让编译器自动适配参数类型
//
// 原理：
//   虽然 Node 和 LifecycleNode 没有继承同一个父类，
//   但它们都实现了同样的接口方法：
//     get_node_base_interface()     → 节点名、命名空间
//     get_node_logging_interface()  → 日志
//     get_node_parameters_interface() → 参数
//     ...等等
//   用模板 + duck typing，编译器会在编译时检查这些方法是否存在。
// =========================================================================


// ===== 方式1（反例）：写死 rclcpp::Node::SharedPtr =====
// 只能接受 rclcpp::Node，不能接受 LifecycleNode 等其他类型
void node_info_restrictive(rclcpp::Node::SharedPtr node)
{
  RCLCPP_INFO(node->get_logger(),
    "【方式1】节点名: %s（只接受 rclcpp::Node）",
    node->get_name());
}


// ===== 方式2（过时）：挨个传接口 SharedPtr =====
// 太啰嗦，要传 N 个参数
void node_info_verbose(
    rclcpp::node_interfaces::NodeBaseInterface::SharedPtr base,
    rclcpp::node_interfaces::NodeLoggingInterface::SharedPtr logging)
{
  RCLCPP_INFO(logging->get_logger(),
    "【方式2】节点名: %s", base->get_name());
}


// ===== 方式3（推荐）：C++ 模板，接受任何 Node 类型 =====
// NodeT 可以是 rclcpp::Node、LifecycleNode、或任何实现了
// get_node_base_interface() 和 get_node_logging_interface() 的类型。
// 编译器会自动匹配。
template<typename NodeT>
void node_info_generic(NodeT & node)
{
  // 从传入的节点中提取需要的接口
  // 这些方法在所有 Node 类型上都有（但不在同一个父类上）
  auto base    = node.get_node_base_interface();
  auto logging = node.get_node_logging_interface();

  RCLCPP_INFO(logging->get_logger(),
    "【方式3】节点名: %s, 命名空间: %s",
    base->get_name(),
    base->get_namespace());
}


// ===== 演示：用模板函数操作参数接口 =====
template<typename NodeT>
void set_and_get_params(NodeT & node)
{
  // 需要哪些接口就调哪些方法
  auto params  = node.get_node_parameters_interface();
  auto logging = node.get_node_logging_interface();

  // 声明参数（模板里调用模板方法要加 .template）
  node.template declare_parameter<double>("speed", 0.0);
  node.template declare_parameter<std::string>("name", "");

  // 设置参数
  params->set_parameters({
    rclcpp::Parameter("speed", 1.5),
    rclcpp::Parameter("name", "robot_A"),
  });

  // 读取参数
  double speed = 0.0;
  node.template get_parameter<double>("speed", speed);
  RCLCPP_INFO(logging->get_logger(), "  speed = %.1f", speed);
}


// ===== 主函数 =====
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("template_demo");

  // ----- 方式1：不灵活 -----
  RCLCPP_INFO(node->get_logger(), "=== 方式1：写死类型 ===");
  node_info_restrictive(node);

  // ----- 方式2：太啰嗦 -----
  RCLCPP_INFO(node->get_logger(), "=== 方式2：挨个传接口 ===");
  node_info_verbose(
    node->get_node_base_interface(),
    node->get_node_logging_interface());

  // ----- 方式3：模板，推荐 -----
  RCLCPP_INFO(node->get_logger(), "=== 方式3：C++ 模板（推荐）===");
  node_info_generic(*node);                     // 传 Node 引用 ✅

  // ----- 方式3 演示参数操作 -----
  RCLCPP_INFO(node->get_logger(), "=== 模板函数操作参数 ===");
  set_and_get_params(*node);

  rclcpp::shutdown();
  return 0;
}
