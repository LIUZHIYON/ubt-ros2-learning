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
# 🐢 经典乌龟追乌龟 demo（仅需 3 个终端）
# 终端 1：启动 turtlesim
ros2 run turtlesim turtlesim_node
# 终端 2：键盘控制乌龟
ros2 run turtlesim turtle_teleop_key
# 终端 3：广播 turtle1
ros2 run learning_tf2_cpp turtle_tf2_broadcaster --ros-args -p turtlename:=turtle1
# 终端 4：全自动 listener（spawn turtle2 + 广播 + 追踪，一键搞定）
ros2 run learning_tf2_cpp turtle_tf2_listener --ros-args -p target_frame:=turtle1

# 或者一键启动 Python 版
ros2 launch learning_tf2_py turtle_tf2_demo_launch.py
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
