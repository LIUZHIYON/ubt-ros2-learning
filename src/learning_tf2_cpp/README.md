# learning_tf2_cpp

TF2 坐标变换教程 C++ 实现，基于 ROS 2 Humble。

## 节点列表

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `static_turtle_tf2_broadcaster` | `static_turtle_tf2_broadcaster` | 发布一个静态坐标变换 |
| `turtle_tf2_broadcaster` | `turtle_tf2_broadcaster` | 根据 turtlesim 的 pose 话题动态发布变换 |
| `turtle_tf2_listener` | `turtle_tf2_frame_listener` | 🌟 全自动：spawn turtle2 + 广播它的 tf + 追踪 target |
| `frame_tf2_broadcaster` | `fixed_frame_tf2_broadcaster` | 定时发布一个附属于 turtle1 的固定子坐标系（carrot1） |

## 如何使用

### 前置条件

```bash
sudo apt install ros-humble-turtlesim

cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select learning_tf2_cpp
source install/setup.bash
```

### 1. 静态广播器

```bash
ros2 run learning_tf2_cpp static_turtle_tf2_broadcaster
```

### 2. 🐢 乌龟追乌龟 demo（仅需 3 个终端！）

```bash
# 终端 1：启动 turtlesim
ros2 run turtlesim turtlesim_node

# 终端 2：键盘控制 turtle1
ros2 run turtlesim turtle_teleop_key

# 终端 3：广播 turtle1 的 tf（广播器）
ros2 run learning_tf2_cpp turtle_tf2_broadcaster --ros-args -p turtlename:=turtle1

# 终端 4：全自动监听器（自动 spawn turtle2 + 广播 + 追踪）
ros2 run learning_tf2_cpp turtle_tf2_listener --ros-args -p target_frame:=turtle1
```

> **listener 会自动完成：**
> 1. 等待 turtlesim 就绪 → spawn 第二只乌龟 `turtle2`
> 2. 订阅 `/turtle2/pose` → 广播 `world → turtle2` 变换
> 3. 查找 `turtle2 → turtle1` 变换 → 发布速度指令让 turtle2 追 turtle1

用键盘移动 turtle1，turtle2 就会自动追过来！

### 3. 动态坐标系（carrot）

```bash
ros2 run learning_tf2_cpp frame_tf2_broadcaster
```

### 参数

| 节点 | 参数 | 默认值 | 说明 |
|---|---|---|---|
| `turtle_tf2_broadcaster` | `turtlename` | `turtle1` | 要广播坐标的乌龟名称 |
| `turtle_tf2_listener` | `target_frame` | `turtle1` | 追踪的目标坐标系 |
