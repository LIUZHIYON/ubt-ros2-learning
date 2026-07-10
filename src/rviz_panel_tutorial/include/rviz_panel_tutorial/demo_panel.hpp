#ifndef RVIZ_PANEL_TUTORIAL__DEMO_PANEL_HPP_
#define RVIZ_PANEL_TUTORIAL__DEMO_PANEL_HPP_

#include <rviz_common/panel.hpp>
#include <rviz_common/ros_integration/ros_node_abstraction_iface.hpp>
#include <std_msgs/msg/string.hpp>
#include <QLabel>
#include <QPushButton>

namespace rviz_panel_tutorial
{

/**
 * DemoPanel: 自定义 RViz 面板插件
 *
 * 功能:
 *   - 左侧标签: 显示从 /input 话题收到的 String 消息
 *   - 右侧按钮: 点击后向 /output 话题发布 "Button clicked!"
 *
 * 核心概念:
 *   RViz 面板是 Qt Widget, 可以在 RViz 布局中自由拖动/停靠。
 *   通过 getDisplayContext() 获取 RViz 内部的 ROS 节点,
 *   用这个节点创建订阅器和发布器（和平常写 rclcpp 完全一样）。
 */
class DemoPanel : public rviz_common::Panel
{
  Q_OBJECT

public:
  explicit DemoPanel(QWidget * parent = 0);
  ~DemoPanel() override;

  // 初始化 ROS 组件（订阅器/发布器）
  void onInitialize() override;

protected:
  // RViz 提供的抽象 ROS 节点指针
  std::shared_ptr<rviz_common::ros_integration::RosNodeAbstractionIface> node_ptr_;

  // 发布到 /output
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

  // 订阅 /input
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

  // 收到 /input 消息时的回调
  void topicCallback(const std_msgs::msg::String & msg);

  // Qt 控件
  QLabel * label_;         // 显示接收到的消息
  QPushButton * button_;   // 点击可发布消息

private Q_SLOTS:
  // 按钮按下时的回调
  void buttonActivated();
};

}  // namespace rviz_panel_tutorial

#endif  // RVIZ_PANEL_TUTORIAL__DEMO_PANEL_HPP_
