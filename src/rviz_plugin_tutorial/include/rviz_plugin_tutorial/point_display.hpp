#ifndef RVIZ_PLUGIN_TUTORIAL__POINT_DISPLAY_HPP_
#define RVIZ_PLUGIN_TUTORIAL__POINT_DISPLAY_HPP_

#include <rviz_common/message_filter_display.hpp>
#include <rviz_common/properties/color_property.hpp>
#include <rviz_rendering/objects/shape.hpp>
#include <rviz_plugin_tutorial_msgs/msg/point2_d.hpp>

namespace rviz_plugin_tutorial
{

/**
 * PointDisplay: 自定义 RViz Display 插件
 * 订阅 Point2D 消息, 在 3D 空间中显示一个彩色方块
 */
class PointDisplay
  : public rviz_common::MessageFilterDisplay<rviz_plugin_tutorial_msgs::msg::Point2D>
{
  Q_OBJECT   // Qt MOC 需要

public:
  PointDisplay() = default;
  ~PointDisplay() override = default;

protected:
  // 初始化：在 RViz 加载插件时调用
  void onInitialize() override;

  // 核心：每收到一条消息就调一次
  void processMessage(
    const rviz_plugin_tutorial_msgs::msg::Point2D::ConstSharedPtr msg) override;

private Q_SLOTS:
  // 当用户在面板上改了颜色, 这个回调被触发
  void updateStyle();

private:
  // 3D 场景中的方块对象
  std::unique_ptr<rviz_rendering::Shape> point_shape_;

  // RViz 面板中的颜色选择器
  std::unique_ptr<rviz_common::properties::ColorProperty> color_property_;
};

}  // namespace rviz_plugin_tutorial

#endif  // RVIZ_PLUGIN_TUTORIAL__POINT_DISPLAY_HPP_
