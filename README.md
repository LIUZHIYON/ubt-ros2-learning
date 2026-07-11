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
| [`cpp_pubsub`](src/cpp_pubsub/) | 发布者/订阅者 + Topic Statistics + linter 集成 |
| [`cpp_parameter_event_handler`](src/cpp_parameter_event_handler/) | 参数变化事件处理 |
| [`composition_demo`](src/composition_demo/) | 组件组合演示（talker + listener 组件） |
| [`node_interfaces_tutorial`](src/node_interfaces_tutorial/) | 节点接口模板类使用 |
| [`learning_tf2_cpp`](src/learning_tf2_cpp/) | TF2 坐标变换教程（广播器 + 监听器） |
| [`urdf_tutorial_cpp`](src/urdf_tutorial_cpp/) | URDF 实操 C++ 版（发布关节状态、TF） |
| [`allocator_demo`](src/allocator_demo/) | 自定义内存分配器（intra-process 对比） |
| [`bag_recorder_demo`](src/bag_recorder_demo/) | rosbag2 C++ API 录制 bag |
| [`bag_reader_demo`](src/bag_reader_demo/) | rosbag2 C++ API 读取 bag |

### 🟨 Python 包

| 包名 | 说明 |
|---|---|
| [`py_pubsub`](src/py_pubsub/) | 发布者/订阅者基础示例 |
| [`py_srvcli`](src/py_srvcli/) | 服务/客户端示例 |
| [`python_parameters`](src/python_parameters/) | 参数使用示例 |
| [`action_tutorials_py`](src/action_tutorials_py/) | Action 客户端/服务端（Fibonacci） |
| [`learning_tf2_py`](src/learning_tf2_py/) | TF2 坐标变换教程 Python 版 |
| [`launch_tutorial`](src/launch_tutorial/) | Launch 文件教程 |
| [`my_package`](src/my_package/) | 自定义节点模板 |
| [`bag_recorder_py`](src/bag_recorder_py/) | rosbag2 Python API 录制 bag |

### 🟩 接口包

| 包名 | 说明 |
|---|---|
| [`tutorial_interfaces`](src/tutorial_interfaces/) | 自定义消息/服务（Num, Sphere, AddThreeInts） |
| [`more_interfaces`](src/more_interfaces/) | 更多自定义接口（AddressBook 消息） |
| [`action_tutorials_interfaces`](src/action_tutorials_interfaces/) | 自定义 Action 接口（Fibonacci） |
| [`rviz_plugin_tutorial_msgs`](src/rviz_plugin_tutorial_msgs/) | 自定义消息 Point2D（RViz 插件教程的接口包） |

### 🎨 RViz / 可视化

| 包名 | 说明 |
|---|---|
| [`visualization_marker_tutorials`](src/visualization_marker_tutorials/) | Marker 基本形状：方块/球/箭头/圆柱 |
| [`points_and_lines_tutorial`](src/points_and_lines_tutorial/) | Marker 点和线：螺旋线/点集/连续线/独立线段 |
| [`marker_display_types_tutorial`](src/marker_display_types_tutorial/) | Marker 全类型演示：12 种 Marker 类型循环展示 |
| [`rviz_plugin_tutorial`](src/rviz_plugin_tutorial/) | RViz 自定义 **Display** 插件：订阅 Point2D → 3D 渲染方块 |
| [`rviz_panel_tutorial`](src/rviz_panel_tutorial/) | RViz 自定义 **Panel** 面板插件：订阅/发布 ROS 消息 |
| [`rqt_bag_diagnostics_demo`](src/rqt_bag_diagnostics_demo/) | rqt_bag 自定义可视化插件（诊断状态着色） |

### 🟪 URDF 学习

| 包名 | 说明 |
|---|---|
| [`my_robot_urdf`](src/my_robot_urdf/) | URDF 从零入门（01~09：基础形状 → 材质 → Gazebo 仿真） |
| [`second_robot`](src/second_robot/) | URDF + robot_state_publisher 教程 |

### ⚙️ 配置

| 文件 | 说明 |
|---|---|
| [`fastdds_profiles.xml`](src/fastdds_profiles.xml) | Fast DDS XML 配置（UDP 调优/参与者限制/异步发布） |

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

## 快速体验

### 🐢 经典乌龟追乌龟

```bash
# 终端 1：启动 turtlesim
ros2 run turtlesim turtlesim_node
# 终端 2：键盘控制乌龟
ros2 run turtlesim turtle_teleop_key
# 终端 3：自动追踪
ros2 run learning_tf2_cpp turtle_tf2_listener --ros-args -p target_frame:=turtle1
```

### 📊 Topic Statistics（消息统计）

```bash
# 终端 1：publisher
ros2 run cpp_pubsub publisher

# 终端 2：带统计的 subscriber
ros2 run cpp_pubsub subscriber_with_stats

# 终端 3：查看统计
ros2 topic echo /statistics
```

### 🧠 自定义内存分配器

```bash
# 普通模式
ros2 run allocator_demo allocator_demo

# Intra-process 模式（系统堆分配大幅减少）
ros2 run allocator_demo allocator_demo intra
```

### 📦 Bag 录制与回放

```bash
# C++ 录制
ros2 run bag_recorder_demo bag_recorder         # 录到 my_bag/
# Python 录制
ros2 run bag_recorder_py bag_recorder_py        # 录到 my_bag_py/

# 命令行录制（通用）
ros2 bag record -o my_bag /topic_name

# 回放
ros2 bag play my_bag/

# 查看信息
ros2 bag info my_bag/

# C++ 程序读取
ros2 run bag_reader_demo bag_reader             # 读取 my_bag/
```

### ⚡ Fast DDS 配置

```bash
FASTDDS_DEFAULT_PROFILES_FILE=~/ros2_ws/src/fastdds_profiles.xml ros2 run cpp_pubsub publisher
```

### 🖼️ RViz 可视化

```bash
rviz2

# Marker 基本形状
ros2 run visualization_marker_tutorials basic_shapes

# 点和线（螺旋线）
ros2 run points_and_lines_tutorial points_and_lines

# 全 12 种 Marker 类型
ros2 run marker_display_types_tutorial all_markers_demo
```

### 🎯 Discovery Server（发现服务器）

```bash
# 终端 1：Server
fastdds discovery -i 0

# 终端 2/3：所有节点加这个环境变量
export ROS_DISCOVERY_SERVER=127.0.0.1:11811
ros2 run demo_nodes_cpp talker
```

### ✅ 代码质量检查（Linter）

```bash
colcon test --packages-select cpp_pubsub
colcon test-result --all
```

## 目录结构

```
ros2_ws/
├── src/              # 源代码（每个包一个目录）
├── build/            # 构建产物（已 gitignore）
├── install/          # 安装目录（已 gitignore）
├── log/              # 日志（已 gitignore）
└── README.md
```
