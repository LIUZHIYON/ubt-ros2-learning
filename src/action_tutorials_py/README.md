# action_tutorials_py

ROS 2 Action 客户端/服务端 Python 示例——斐波那契数列。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `fibonacci_action_server` | `fibonacci_action_server` | Action 服务端，按需计算斐波那契数列 |
| `fibonacci_action_client` | `fibonacci_action_client` | Action 客户端，发送目标并接收反馈和结果 |

## 依赖

- `action_tutorials_interfaces`（自定义 Fibonacci Action）

## 运行

```bash
# 先构建依赖
colcon build --packages-select action_tutorials_interfaces action_tutorials_py
source install/setup.bash

# 启动服务端
ros2 run action_tutorials_py fibonacci_action_server

# 另一个终端：启动客户端（发送 order=10 的目标）
ros2 run action_tutorials_py fibonacci_action_client --ros-args -p order:=10
```

客户端会实时打印反馈（部分数列），最终显示完整的斐波那契数列结果。
