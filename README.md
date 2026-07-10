# ubt-ros2-learning

ROS 2 学习工作空间，基于 ROS 2 Humble。

## 环境

- **ROS 2 版本：** Humble
- **构建工具：** colcon
- **语言：** C++ / Python

## 包列表

### 🟦 C++ 包

| 包名 | 说明 |
|---|---|
| [`cpp_pubsub`](src/cpp_pubsub/README.md) | 发布者/订阅者基础示例（自定义 Num 消息） |
| [`py_srvcli`](src/py_srvcli/README.md) | 服务/客户端示例（自定义 AddThreeInts） |
| [`cpp_parameter_event_handler`](src/cpp_parameter_event_handler/README.md) | 参数变化事件处理 |
| [`composition_demo`](src/composition_demo/README.md) | 组件组合演示（talker + listener 组件） |
| [`node_interfaces_tutorial`](src/node_interfaces_tutorial/README.md) | 节点接口模板类使用 |
| [`learning_tf2_cpp`](src/learning_tf2_cpp/README.md) | TF2 坐标变换教程（广播器 + 监听器） |

### 🟨 Python 包

| 包名 | 说明 |
|---|---|
| [`py_pubsub`](src/py_pubsub/README.md) | 发布者/订阅者基础示例 |
| [`py_srvcli`](src/py_srvcli/README.md) | 服务/客户端示例 |
| [`python_parameters`](src/python_parameters/README.md) | 参数使用示例 |
| [`action_tutorials_py`](src/action_tutorials_py/README.md) | Action 客户端/服务端（Fibonacci） |
| [`learning_tf2_py`](src/learning_tf2_py/README.md) | TF2 坐标变换教程 Python 版 |
| [`launch_tutorial`](src/launch_tutorial/README.md) | Launch 文件教程 |
| [`my_package`](src/my_package/README.md) | 自定义节点模板 |

### 🟩 接口包

| 包名 | 说明 |
|---|---|
| [`tutorial_interfaces`](src/tutorial_interfaces/README.md) | 自定义消息/服务（Num, Sphere, AddThreeInts） |
| [`more_interfaces`](src/more_interfaces/README.md) | 更多自定义接口（AddressBook 消息） |
| [`action_tutorials_interfaces`](src/action_tutorials_interfaces/README.md) | 自定义 Action 接口（Fibonacci） |

### 🟪 URDF 学习

| 包名 | 说明 |
|---|---|
| [`my_robot_urdf`](src/my_robot_urdf/README.md) | URDF 从零入门（01~09：基础形状 → 材质 → Gazebo 仿真） |
| [`second_robot`](src/second_robot/README.md) | URDF + robot_state_publisher 教程 |
| [`urdf_tutorial_cpp`](src/urdf_tutorial_cpp/README.md) | URDF 实操 C++ 版（发布关节状态、TF） |

### 🎨 RViz / 可视化

| 包名 | 说明 |
|---|---|
| [`visualization_marker_tutorials`](src/visualization_marker_tutorials/) | Marker 基本形状：方块/球/箭头/圆柱 |
| [`points_and_lines_tutorial`](src/points_and_lines_tutorial/) | Marker 点和线：螺旋线/点集/连续线/独立线段 |
| [`marker_display_types_tutorial`](src/marker_display_types_tutorial/) | Marker 全类型演示：12 种 Marker 类型循环展示 |
| [`rviz_plugin_tutorial_msgs`](src/rviz_plugin_tutorial_msgs/) | 自定义消息 Point2D（RViz 插件教程的接口包） |
| [`rviz_plugin_tutorial`](src/rviz_plugin_tutorial/) | RViz 自定义 **Display** 插件：订阅 Point2D → 3D 渲染方块 |
| [`rviz_panel_tutorial`](src/rviz_panel_tutorial/) | RViz 自定义 **Panel** 面板插件：订阅/发布 ROS 消息 |

### 📦 外部/submodule

| 包名 | 说明 |
|---|---|
| `ros_tutorials` | ROS 官方教程（turtlesim 等） |

## 构建

```bash
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build
source install/setup.bash
```

### 单独构建某个包

```bash
colcon build --packages-select <package_name>
```

## 快速体验

```bash
# 🐢 经典乌龟追乌龟 demo
# 终端 1：启动 turtlesim
ros2 run turtlesim turtlesim_node
# 终端 2：键盘控制乌龟
ros2 run turtlesim turtle_teleop_key
# 终端 3：全自动 listener（spawn turtle2 + 广播 + 追踪）
ros2 run learning_tf2_cpp turtle_tf2_listener --ros-args -p target_frame:=turtle1
# 或者一键启动 Python 版
ros2 launch learning_tf2_py turtle_tf2_demo_launch.py
```

### 🖼️ RViz 可视化

```bash
# 终端 1：启动 RViz
rviz2

# Marker 基本形状（方块/球/箭头/圆柱）
ros2 run visualization_marker_tutorials basic_shapes

# 点和线（螺旋线）
ros2 run points_and_lines_tutorial points_and_lines

# 全 12 种 Marker 类型演示
ros2 run marker_display_types_tutorial all_markers_demo

# 自定义 Display 插件（需先在 RViz 里 Add → Point2D）
ros2 topic pub /point rviz_plugin_tutorial_msgs/msg/Point2D \
  "{header: {frame_id: map}, x: 1.0, y: 2.0}" -r 1

# 自定义 Panel 面板（RViz 菜单 → Panels → Add New Panel → DemoPanel）
ros2 topic pub /input std_msgs/msg/String "{data: 'Hello RViz'}"
ros2 topic echo /output
```

> **注意：** RViz 的 Fixed Frame 需要设为代码里对应的 frame_id（如 `my_frame`、`map`）

## 目录结构

```
ros2_ws/
├── src/              # 源代码（每个包一个目录）
├── build/            # 构建产物（已 gitignore）
├── install/          # 安装目录（已 gitignore）
├── log/              # 日志（已 gitignore）
└── README.md
```
