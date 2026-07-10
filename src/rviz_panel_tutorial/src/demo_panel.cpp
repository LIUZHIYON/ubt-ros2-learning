#include "rviz_panel_tutorial/demo_panel.hpp"

#include <QVBoxLayout>
#include <rviz_common/display_context.hpp>

namespace rviz_panel_tutorial
{

DemoPanel::DemoPanel(QWidget * parent)
: rviz_common::Panel(parent)
{
  // ===== 构建 UI 布局 =====
  // QVBoxLayout: 垂直排列控件
  auto * layout = new QVBoxLayout(this);

  // 创建标签和按钮
  label_ = new QLabel("[等待消息...]");
  button_ = new QPushButton("发送!");

  // 添加到布局
  layout->addWidget(label_);
  layout->addWidget(button_);

  // ===== 连接信号/槽 =====
  // 按钮松开 → 调用 buttonActivated()
  QObject::connect(
    button_, &QPushButton::released,
    this, &DemoPanel::buttonActivated);
}

DemoPanel::~DemoPanel() = default;

void DemoPanel::onInitialize()
{
  // ===== 获取 RViz 内部的 ROS 节点 =====
  // getDisplayContext() → getRosNodeAbstraction() 拿到抽象节点
  // .lock() 返回 shared_ptr，锁定期间独占使用
  node_ptr_ = getDisplayContext()->getRosNodeAbstraction().lock();

  // 转换为熟悉的 rclcpp::Node 来创建订阅器/发布器
  rclcpp::Node::SharedPtr node = node_ptr_->get_raw_node();

  // 发布器：向 /output 发 String 消息
  publisher_ = node->create_publisher<std_msgs::msg::String>("/output", 10);

  // 订阅器：监听 /input 话题，收到消息调 topicCallback
  subscription_ = node->create_subscription<std_msgs::msg::String>(
    "/input", 10,
    std::bind(&DemoPanel::topicCallback, this, std::placeholders::_1));
}

// ===== 订阅回调: 收到 /input 消息 → 更新标签文字 =====
void DemoPanel::topicCallback(const std_msgs::msg::String & msg)
{
  label_->setText(QString::fromStdString(msg.data));
}

// ===== 按钮回调: 向 /output 发布消息 =====
void DemoPanel::buttonActivated()
{
  auto message = std_msgs::msg::String();
  message.data = "Button clicked!";
  publisher_->publish(message);
}

}  // namespace rviz_panel_tutorial

// ===== 插件注册（必须是 Panel）=====
#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(rviz_panel_tutorial::DemoPanel, rviz_common::Panel)
