# learning_tf2_cpp

TF2 坐标变换教程 C++ 实现，基于 ROS 2 Humble。

## 节点列表

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `static_turtle_tf2_broadcaster` | `static_turtle_tf2_broadcaster` | 发布一个静态坐标变换 |
| `turtle_tf2_broadcaster` | `turtle_tf2_broadcaster` | 根据 turtlesim 的 pose 话题动态发布变换 |
| `turtle_tf2_listener` | `turtle_tf2_frame_listener` | 监听 tf 变换并控制 turtle2 追踪目标 |
| `frame_tf2_broadcaster` | `fixed_frame_tf2_broadcaster` | 定时发布一个附属于 turtle1 的固定子坐标系（carrot1） |

## 如何使用

### 前置条件

```bash
# 确认 turtlesim 已安装
sudo apt install ros-humble-turtlesim

# 构建
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select learning_tf2_cpp
source install/setup.bash
```

### 1. 静态广播器

发布一个不动的 `world → turtle1` 变换：

```bash
ros2 run learning_tf2_cpp static_turtle_tf2_broadcaster
```

### 2. 动态广播器 + 监听器（经典「乌龟追乌龟」demo）

需要 6 个终端协同运行：

```bash
# 终端 1：启动 turtlesim
ros2 run turtlesim turtlesim_node

# 终端 2：键盘控制 turtle1
ros2 run turtlesim turtle_teleop_key

# 终端 3：广播 turtle1 的坐标变换
ros2 run learning_tf2_cpp turtle_tf2_broadcaster --ros-args -p turtlename:=turtle1

# 终端 4：创建第二只乌龟
ros2 service call /spawn turtlesim/srv/Spawn "{x: 5.5, y: 5.5, theta: 0.0, name: 'turtle2'}"

# 终端 5：广播 turtle2 的坐标变换
ros2 run learning_tf2_cpp turtle_tf2_broadcaster --ros-args -p turtlename:=turtle2

# 终端 6：监听器让 turtle2 自动追 turtle1
ros2 run learning_tf2_cpp turtle_tf2_listener --ros-args -p target_frame:=turtle1
```

用键盘移动 turtle1，turtle2 会自动追上来。

### 3. 动态坐标系（carrot）

在 turtle1 的前方（x 偏移 2.0）添加一个 `carrot1` 子坐标系：

```bash
ros2 run learning_tf2_cpp frame_tf2_broadcaster
```

### 参数

| 节点 | 参数 | 默认值 | 说明 |
|---|---|---|---|
| `turtle_tf2_broadcaster` | `turtlename` | `turtle1` | 要广播坐标的乌龟名称 |
| `turtle_tf2_listener` | `target_frame` | `turtle1` | 追踪的目标坐标系 |
