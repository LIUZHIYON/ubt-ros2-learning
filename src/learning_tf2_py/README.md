# learning_tf2_py

TF2 坐标变换教程 Python 实现，与 C++ 版的 `learning_tf2_cpp` 对应。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `static_turtle_tf2_broadcaster` | `static_turtle_tf2_broadcaster` | 发布静态变换 world→turtle1 |
| `turtle_tf2_broadcaster` | `turtle_tf2_broadcaster` | 根据 turtlesim pose 动态广播变换 |
| `turtle_tf2_listener` | `turtle_tf2_frame_listener` | 监听变换，控制 turtle2 追 turtle1 |
| `fixed_frame_tf2_broadcaster` | `fixed_frame_tf2_broadcaster` | 发布 turtle1→carrot1 固定偏移 |
| `dynamic_frame_tf2_broadcaster` | `dynamic_frame_tf2_broadcaster` | 发布 turtle1→carrot1 动态旋转偏移 |

## Launch 文件

| 文件 | 说明 |
|---|---|
| `turtle_tf2_demo_launch.py` | 一键启动完整 demo（turtlesim + 广播器 + 监听器） |
| `turtle_tf2_fixed_frame_demo_launch.py` | 固定坐标系 demo |
| `turtle_tf2_dynamic_frame_demo_launch.py` | 动态坐标系 demo |

## 运行

```bash
# 一键启动完整乌龟追乌龟 demo
ros2 launch learning_tf2_py turtle_tf2_demo_launch.py

# 动态坐标系 demo
ros2 launch learning_tf2_py turtle_tf2_dynamic_frame_demo_launch.py
```
