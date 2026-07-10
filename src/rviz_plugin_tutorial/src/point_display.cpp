#include "rviz_plugin_tutorial/point_display.hpp"
#include <rviz_common/logging.hpp>
#include <rviz_common/properties/parse_color.hpp>
#include <rviz_common/properties/status_property.hpp>

using rviz_common::properties::StatusProperty;

namespace rviz_plugin_tutorial
{

void PointDisplay::onInitialize()
{
  // MFDClass = MessageFilterDisplay 的简写（父类定义的别名）
  MFDClass::onInitialize();

  // 在 3D 场景中创建一个方块
  // 必须在 onInitialize 里创建, 因为此刻 scene_manager_ / scene_node_ 才可用
  point_shape_ = std::make_unique<rviz_rendering::Shape>(
    rviz_rendering::Shape::Type::Cube, scene_manager_, scene_node_);

  // 添加颜色属性到 RViz 面板
  // 参数: 名称, 默认颜色, 描述, 父对象, 回调的 SLOT
  color_property_ = std::make_unique<rviz_common::properties::ColorProperty>(
    "Point Color", QColor(36, 64, 142),
    "Color to draw the point.",
    this, SLOT(updateStyle()));

  // 立即应用默认颜色
  updateStyle();
}

void PointDisplay::processMessage(
  const rviz_plugin_tutorial_msgs::msg::Point2D::ConstSharedPtr msg)
{
  RVIZ_COMMON_LOG_INFO_STREAM(
    "收到 Point2D 消息, frame=" << msg->header.frame_id
    << " x=" << msg->x << " y=" << msg->y);

  // ========== 步骤 1: 坐标系变换 ==========
  // 把消息的 frame 变换到 RViz 的 Fixed Frame
  Ogre::Vector3 position;
  Ogre::Quaternion orientation;
  if (!context_->getFrameManager()->getTransform(
        msg->header, position, orientation))
  {
    // 变换失败 → 丢弃此消息
    RVIZ_COMMON_LOG_DEBUG_STREAM(
      "无法从 '" << msg->header.frame_id
      << "' 变换到 '" << qPrintable(fixed_frame_) << "'");
    return;
  }

  // 把整个 scene_node_ 放到正确位置
  scene_node_->setPosition(position);
  scene_node_->setOrientation(orientation);

  // ========== 步骤 2: 设置方块位置 ==========
  Ogre::Vector3 point_pos;
  point_pos.x = msg->x;
  point_pos.y = msg->y;
  point_pos.z = 0.0;
  point_shape_->setPosition(point_pos);

  // ========== 步骤 3: 状态提示（好玩的功能）==========
  if (msg->x < 0) {
    setStatus(StatusProperty::Warn, "Message",
      "我抱怨一下: x 坐标是负数!");
  } else {
    setStatus(StatusProperty::Ok, "Message", "OK");
  }
}

void PointDisplay::updateStyle()
{
  // 从面板读取颜色, 转换为 OGRE 格式, 应用到方块
  Ogre::ColourValue color =
    rviz_common::properties::qtToOgre(color_property_->getColor());
  point_shape_->setColor(color);
}

}  // namespace rviz_plugin_tutorial

// ========== 插件注册宏（必须！）==========
#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(rviz_plugin_tutorial::PointDisplay, rviz_common::Display)
