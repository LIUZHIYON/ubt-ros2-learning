# launch_tutorial

ROS 2 Launch 文件教程，演示各种 launch 文件的写法。

## Launch 文件

| 文件名 | 说明 |
|---|---|
| `turtlesim_world_1_launch.py` | 基础 launch：启动 turtlesim + mimic |
| `turtlesim_world_2_launch.py` | 多个 launch 文件：拆分 turtlesim 和 mimic |
| `turtlesim_world_3_launch.py` | 带参数 + push-ros-namespace |
| `example_main.launch.py` | launch 文件嵌套 |
| `example_substitutions.launch.py` | 启动参数替换（launch argument → 传递到节点参数） |
| `example_event_handlers.launch.py` | 事件处理器（OnProcessExit 等） |
| `turtlesim_rviz_launch.py` | 同时启动 turtlesim 和 RViz |
| `fixed_broadcaster_launch.py` | TF2 固定坐标系广播 launch |
| `broadcaster_listener_launch.py` | TF2 广播器 + 监听器 launch |
| `mimic_launch.py` | 多节点 mimic 示例 |

## 运行

```bash
# 最基础的例子
ros2 launch launch_tutorial turtlesim_world_1_launch.py

# 带参数
ros2 launch launch_tutorial turtlesim_world_3_launch.py background_r:=255
```
