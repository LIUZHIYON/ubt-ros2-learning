# ubt-ros2-learning

ROS2 学习工作空间，基于 ROS2 Humble。

## 环境

- **ROS2 版本：** Humble
- **构建工具：** colcon
- **语言：** C++ / Python

## 包列表

| 包名 | 类型 | 说明 |
|------|------|------|
| `cpp_pubsub` | C++ | 发布者/订阅者基础示例 |
| `py_pubsub` | Python | 发布者/订阅者基础示例 |
| `py_srvcli` | Python | 服务/客户端示例 |
| `cpp_parameter_event_handler` | C++ | 参数事件处理 |
| `python_parameters` | Python | Python 参数使用示例 |
| `tutorial_interfaces` | C++ | 自定义消息/服务接口（Num, Sphere, AddThreeInts） |
| `more_interfaces` | C++ | 更多自定义接口（AddressBook） |
| `action_tutorials_interfaces` | C++ | 自定义 Action 接口（Fibonacci） |
| `action_tutorials_py` | Python | Action 客户端/服务端示例 |
| `node_interfaces_tutorial` | C++ | 节点接口示例 |
| `composition_demo` | C++ | 组件组合演示 |
| `launch_tutorial` | Python | Launch 文件教程 |
| `learning_tf2_py` | Python | TF2 坐标变换学习 |
| `my_package` | Python | 自定义节点模板 |
| `ros_tutorials` | submodule | ROS 官方教程 |

## 使用

```bash
# 构建
cd ~/ros2_ws
colcon build

# 设置环境
source install/setup.bash

# 运行示例（以 cpp_pubsub 为例）
ros2 run cpp_pubsub talker
ros2 run cpp_pubsub listener
```

## 目录结构

```
ros2_ws/
├── src/          # 源代码
├── build/        # 构建产物（已 gitignore）
├── install/      # 安装目录（已 gitignore）
├── log/          # 日志（已 gitignore）
├── launch/       # 顶层 launch 文件
└── README.md
```
