# action_tutorials_interfaces

自定义 ROS 2 Action 接口——Fibonacci。

## 接口定义

- **Action：** `Fibonacci.action` — 计算斐波那契数列
  - Goal: `uint32 order` — 要算到第几项
  - Result: `int32[] sequence` — 结果数列
  - Feedback: `int32[] partial_sequence` — 计算中的部分结果

## 构建

```bash
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select action_tutorials_interfaces
source install/setup.bash
```

## 使用

这是一个纯接口包，没有可执行文件。它被 `action_tutorials_py` 依赖使用。
