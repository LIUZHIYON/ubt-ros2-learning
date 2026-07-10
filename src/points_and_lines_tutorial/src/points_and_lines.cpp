#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "visualization_msgs/msg/marker.hpp"

/**
 * Marker: Points and Lines 教程
 *
 * 本节点同时发布三种 Marker 类型：
 *   - POINTS：    在每个顶点放置一个点（绿色圆点）
 *   - LINE_STRIP： 连接所有顶点，形成一条连续的线（蓝色）
 *   - LINE_LIST：  每两个点组成一段独立的线段（红色竖线）
 *
 * 效果：一个旋转的螺旋线 + 每个顶点冒出一根红色竖线
 */
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("points_and_lines");
  auto marker_pub = node->create_publisher<visualization_msgs::msg::Marker>(
    "visualization_marker", 10);
  rclcpp::Rate loop_rate(30);  // 30Hz，让动画流畅

  float f = 0.0f;
  while (rclcpp::ok()) {
    // ========== 创建三个 Marker 消息 ==========
    // 每个 Marker 类型不同，但共享 frame_id / ns / action
    visualization_msgs::msg::Marker points, line_strip, line_list;

    // ---- 头部（三个 Marker 共用）----
    points.header.frame_id =
      line_strip.header.frame_id =
      line_list.header.frame_id = "my_frame";

    points.header.stamp =
      line_strip.header.stamp =
      line_list.header.stamp = node->now();

    // ---- 命名空间（三个共用，但 id 不同避免覆盖）----
    points.ns = line_strip.ns = line_list.ns = "points_and_lines";

    // ---- 动作：ADD = 创建/修改 ----
    points.action = line_strip.action = line_list.action =
      visualization_msgs::msg::Marker::ADD;

    // ---- ID（三个不同的 id，这样不会互相覆盖）----
    points.id = 0;
    line_strip.id = 1;
    line_list.id = 2;

    // ========== 设置类型 ==========
    // POINTS：      在每个点坐标处画一个点
    // LINE_STRIP：  点0→点1→点2→...顺序连成线
    // LINE_LIST：   每两个点一组 (0→1, 2→3, 4→5) 画独立线段
    points.type = visualization_msgs::msg::Marker::POINTS;
    line_strip.type = visualization_msgs::msg::Marker::LINE_STRIP;
    line_list.type = visualization_msgs::msg::Marker::LINE_LIST;

    // ========== 尺寸含义不同！==========
    // POINTS:      scale.x = 宽度, scale.y = 高度
    // LINE_STRIP:  只用 scale.x = 线宽
    // LINE_LIST:   只用 scale.x = 线宽
    // 单位：米
    points.scale.x = 0.2;   // 点宽 20cm
    points.scale.y = 0.2;   // 点高 20cm

    line_strip.scale.x = 0.1;  // 线宽 10cm
    line_list.scale.x = 0.1;   // 线宽 10cm

    // ========== 设颜色（RGBA, 0~1）==========
    // ⚠️ alpha 不设的话默认是 0（完全透明）！

    // 点：绿色
    points.color.r = 0.0f;
    points.color.g = 1.0f;
    points.color.b = 0.0f;
    points.color.a = 1.0;

    // 连续线：蓝色
    line_strip.color.r = 0.0f;
    line_strip.color.g = 0.0f;
    line_strip.color.b = 1.0f;
    line_strip.color.a = 1.0;

    // 独立线段：红色
    line_list.color.r = 1.0f;
    line_list.color.g = 0.0f;
    line_list.color.b = 0.0f;
    line_list.color.a = 1.0;

    // ========== 生成螺旋线的顶点坐标 ==========
    // 用 sin/cos 生成 100 个顶点，X 轴从 -50 到 +49
    // f 不断增大 → 螺旋线旋转
    for (uint32_t i = 0; i < 100; ++i) {
      float y = 5 * sin(f + i / 100.0f * 2 * M_PI);
      float z = 5 * cos(f + i / 100.0f * 2 * M_PI);

      geometry_msgs::msg::Point p;
      p.x = (int32_t)i - 50;  // x: -50 ~ +49
      p.y = y;                 // y: 正弦波，幅度 5
      p.z = z;                 // z: 余弦波，幅度 5

      // POINTS 和 LINE_STRIP：每个顶点只要一个 Point
      points.points.push_back(p);
      line_strip.points.push_back(p);

      // LINE_LIST：每段独立线需要 2 个 Point（起点 + 终点）
      // 这里画从 p 往上 (z+1) 的竖线
      line_list.points.push_back(p);
      p.z += 1.0;              // 往上延伸 1 米
      line_list.points.push_back(p);
    }

    // ========== 发布三个 Marker ==========
    marker_pub->publish(points);      // 绿色点阵
    marker_pub->publish(line_strip);  // 蓝色螺旋线
    marker_pub->publish(line_list);   // 红色竖线段

    loop_rate.sleep();  // 30Hz
    f += 0.04f;         // 改变相位 → 螺旋线旋转
  }

  rclcpp::shutdown();
  return 0;
}
