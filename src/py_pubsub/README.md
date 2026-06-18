# py_pubsub

ROS 2 发布者/订阅者 Python 基础示例。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `talker` | `minimal_publisher` | 定时发布 `std_msgs/msg/String` |
| `listener` | `minimal_subscriber` | 订阅并打印消息 |

## 运行

```bash
ros2 run py_pubsub talker
# 另一个终端
ros2 run py_pubsub listener
```
