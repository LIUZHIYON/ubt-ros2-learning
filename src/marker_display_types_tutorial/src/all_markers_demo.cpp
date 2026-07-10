/**
 * Marker Display Types 全类型演示
 *
 * 按顺序演示 RViz 支持的 12 种 Marker 类型。
 * 每种类型显示 3 秒，左下角有类型编号和名称的文字标签。
 *
 * 类型编号对照:
 *   0=ARROW   1=CUBE   2=SPHERE   3=CYLINDER
 *   4=LINE_STRIP  5=LINE_LIST  6=CUBE_LIST
 *   7=SPHERE_LIST  8=POINTS  9=TEXT_VIEW_FACING
 *  10=MESH_RESOURCE  11=TRIANGLE_LIST
 */

#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "visualization_msgs/msg/marker.hpp"

using namespace std::chrono_literals;

// 类型名称对照表
const char * TYPE_NAMES[] = {
  "ARROW",
  "CUBE",
  "SPHERE",
  "CYLINDER",
  "LINE_STRIP",
  "LINE_LIST",
  "CUBE_LIST",
  "SPHERE_LIST",
  "POINTS",
  "TEXT_VIEW_FACING",
  "MESH_RESOURCE (需要mesh文件, 已跳过)",
  "TRIANGLE_LIST"
};

/**
 * 批量生成 Marker，放到网格上
 * 排版: 每个类型独占一个区域展示
 */
void make_arrow(visualization_msgs::msg::Marker & m)
{
  // --- ARROW: 用 start/end points 方式 ---
  m.type = visualization_msgs::msg::Marker::ARROW;
  m.scale.x = 0.1;  // 杆直径
  m.scale.y = 0.3;  // 箭头直径
  m.scale.z = 0.3;  // 箭头长度

  geometry_msgs::msg::Point start, end;
  start.x = -2.0; start.y = 0.0; start.z = 0.0;
  end.x   =  2.0; end.y   = 0.0; end.z   = 0.0;
  m.points = {start, end};
}

void make_cube(visualization_msgs::msg::Marker & m)
{
  m.type = visualization_msgs::msg::Marker::CUBE;
  m.scale.x = 1.0; m.scale.y = 1.0; m.scale.z = 1.0;
  m.pose.position.x = 0.0; m.pose.position.y = 0.0; m.pose.position.z = 0.0;
}

void make_sphere(visualization_msgs::msg::Marker & m)
{
  m.type = visualization_msgs::msg::Marker::SPHERE;
  m.scale.x = 1.5; m.scale.y = 1.5; m.scale.z = 1.5;
}

void make_cylinder(visualization_msgs::msg::Marker & m)
{
  m.type = visualization_msgs::msg::Marker::CYLINDER;
  m.scale.x = 1.0; m.scale.y = 1.0; m.scale.z = 2.0;
}

void make_line_strip(visualization_msgs::msg::Marker & m)
{
  m.type = visualization_msgs::msg::Marker::LINE_STRIP;
  m.scale.x = 0.08;  // 线宽
  for (int i = 0; i < 20; ++i) {
    geometry_msgs::msg::Point p;
    p.x = (i - 10) * 0.3;
    p.y = sin(i * 0.8) * 1.5;
    p.z = cos(i * 0.8) * 1.0;
    m.points.push_back(p);
  }
}

void make_line_list(visualization_msgs::msg::Marker & m)
{
  m.type = visualization_msgs::msg::Marker::LINE_LIST;
  m.scale.x = 0.05;
  for (int i = 0; i < 10; ++i) {
    geometry_msgs::msg::Point a, b;
    a.x = (i - 5) * 0.6;
    a.y = -1.0; a.z = 0.0;
    b.x = (i - 5) * 0.6;
    b.y =  1.0; b.z = 0.0;
    m.points.push_back(a);
    m.points.push_back(b);
  }
}

void make_cube_list(visualization_msgs::msg::Marker & m)
{
  // CUBE_LIST: 很多一样大的方块, 通过 points 指定位置, 渲染效率高
  m.type = visualization_msgs::msg::Marker::CUBE_LIST;
  m.scale.x = 0.2; m.scale.y = 0.2; m.scale.z = 0.2;
  for (int r = 0; r < 5; ++r) {
    for (int c = 0; c < 5; ++c) {
      geometry_msgs::msg::Point p;
      p.x = (c - 2) * 0.6;
      p.y = (r - 2) * 0.6;
      p.z = 0.0;
      m.points.push_back(p);
    }
  }
}

void make_sphere_list(visualization_msgs::msg::Marker & m)
{
  // SPHERE_LIST: 批量球, 通过 points 指定位置
  m.type = visualization_msgs::msg::Marker::SPHERE_LIST;
  m.scale.x = 0.2; m.scale.y = 0.2; m.scale.z = 0.2;
  for (int r = 0; r < 5; ++r) {
    for (int c = 0; c < 5; ++c) {
      geometry_msgs::msg::Point p;
      p.x = (c - 2) * 0.6;
      p.y = (r - 2) * 0.6;
      p.z = 0.0;
      m.points.push_back(p);
    }
  }
}

void make_points(visualization_msgs::msg::Marker & m)
{
  m.type = visualization_msgs::msg::Marker::POINTS;
  m.scale.x = 0.15; m.scale.y = 0.15;
  for (int i = 0; i < 30; ++i) {
    geometry_msgs::msg::Point p;
    p.x = (i - 15) * 0.4;
    p.y = sin(i * 0.4) * 1.8;
    p.z = cos(i * 0.4) * 0.8;
    m.points.push_back(p);
  }
}

void make_text(visualization_msgs::msg::Marker & m, const std::string & text)
{
  m.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
  m.scale.z = 0.5;  // 大写字母 "A" 的高度 (米)
  m.text = text;
  m.pose.position.x = -3.5;
  m.pose.position.y = -3.0;
  m.pose.position.z =  1.5;
  // 文字始终面向摄像头, 不需要设置 orientation
}

void make_triangle_list(visualization_msgs::msg::Marker & m)
{
  // TRIANGLE_LIST: 每 3 个点构成一个三角形面片
  m.type = visualization_msgs::msg::Marker::TRIANGLE_LIST;
  m.scale.x = 1.0; m.scale.y = 1.0; m.scale.z = 1.0;

  // 金字塔(4个三角形)
  geometry_msgs::msg::Point top;      // 顶点
  top.x = 0.0; top.y = 0.0; top.z = 2.0;

  geometry_msgs::msg::Point bl, br, tl, tr;  // 底面四个角
  bl.x = -1.0; bl.y = -1.0; bl.z = 0.0;
  br.x =  1.0; br.y = -1.0; br.z = 0.0;
  tl.x = -1.0; tl.y =  1.0; tl.z = 0.0;
  tr.x =  1.0; tr.y =  1.0; tl.z = 0.0;

  // 正面
  m.points.push_back(bl); m.points.push_back(br); m.points.push_back(top);
  // 右面
  m.points.push_back(br); m.points.push_back(tr); m.points.push_back(top);
  // 背面
  m.points.push_back(tr); m.points.push_back(tl); m.points.push_back(top);
  // 左面
  m.points.push_back(tl); m.points.push_back(bl); m.points.push_back(top);
}


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("all_markers_demo");
  auto pub = node->create_publisher<visualization_msgs::msg::Marker>(
    "visualization_marker", 10);

  rclcpp::WallRate rate(0.33);  // 每 3 秒切一个类型

  // 跳过第 10 号 (MESH_RESOURCE), 因为没有 mesh 文件
  int type_index = 0;

  RCLCPP_INFO(node->get_logger(),
    "=== RViz Marker 12 类型全演示 ===\n"
    "每种类型显示 3 秒, 左下角有类型名称\n"
    "在 RViz 中添加 Marker Display, Fixed Frame 设为 my_frame\n");

  while (rclcpp::ok()) {
    // 跳过 MESH_RESOURCE (10), 需要 mesh 文件才能显示
    if (type_index == 10) { type_index = 11; }

    // ---- 构造 Marker ----
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "my_frame";
    marker.header.stamp = node->now();
    marker.ns = "type_demo";
    marker.id = 0;                   // 每种类型复用同一个 id
    marker.action = visualization_msgs::msg::Marker::ADD;

    // 默认颜色: 浅蓝
    marker.color.r = 0.2f;
    marker.color.g = 0.6f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    marker.pose.position.x = 0.0;
    marker.pose.position.y = 0.0;
    marker.pose.position.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.lifetime = rclcpp::Duration::from_nanoseconds(0);

    // ---- 根据类型填充数据 ----
    switch (type_index) {
      case 0:  make_arrow(marker);        break;
      case 1:  make_cube(marker);         break;
      case 2:  make_sphere(marker);       break;
      case 3:  make_cylinder(marker);     break;
      case 4:  make_line_strip(marker);   break;
      case 5:  make_line_list(marker);    break;
      case 6:  make_cube_list(marker);    break;
      case 7:  make_sphere_list(marker);  break;
      case 8:  make_points(marker);       break;
      case 9:  make_text(marker,
                  "09: TEXT_VIEW_FACING\n"
                  "始终面向摄像头");      break;
      case 11: make_triangle_list(marker); break;
      default: break;
    }

    pub->publish(marker);

    // ---- 发布类型名称标签（另一个 Marker, id=1）----
    visualization_msgs::msg::Marker label;
    label.header.frame_id = "my_frame";
    label.header.stamp = node->now();
    label.ns = "type_demo";
    label.id = 1;
    label.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
    label.action = visualization_msgs::msg::Marker::ADD;
    label.scale.z = 0.5;
    label.color.r = 1.0f;
    label.color.g = 1.0f;
    label.color.b = 1.0f;
    label.color.a = 1.0;
    label.pose.position.x = -4.0;
    label.pose.position.y = -3.0;
    label.pose.position.z =  2.0;
    label.pose.orientation.w = 1.0;
    label.lifetime = rclcpp::Duration::from_nanoseconds(0);

    char buf[64];
    snprintf(buf, sizeof(buf), "%02d: %s", type_index, TYPE_NAMES[type_index]);
    label.text = buf;
    pub->publish(label);

    RCLCPP_INFO(node->get_logger(),
      ">>> 类型 %02d: %s", type_index, TYPE_NAMES[type_index]);

    // 下一个类型
    type_index++;
    if (type_index > 11) {
      type_index = 0;
      RCLCPP_INFO(node->get_logger(), "--- 一轮演示完毕, 循环 ---\n");
    }

    rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
