# python_parameters

ROS 2 Python 参数使用示例。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `minimal_param_node` | `minimal_param_node` | 声明并读取字符串参数 `my_parameter` |

## 运行

```bash
# 使用默认参数
ros2 run python_parameters minimal_param_node

# 覆盖参数
ros2 run python_parameters minimal_param_node --ros-args -p my_parameter:="Hello World"

# 或通过 launch 文件
ros2 launch python_parameters python_parameters_launch.py
```
