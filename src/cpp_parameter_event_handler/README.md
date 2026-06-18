# cpp_parameter_event_handler

ROS 2 参数事件处理 C++ 示例。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `parameter_event_handler` | `parameter_event_handler` | 注册参数变化回调，当参数被修改时自动触发 |

## 运行

```bash
ros2 run cpp_parameter_event_handler parameter_event_handler

# 另一个终端：修改参数触发回调
ros2 param set /parameter_event_handler my_parameter 42
```

节点会在参数被设置或变化时打印日志。
