# cpp_pubsub

ROS 2 发布者/订阅者 C++ 基础示例（主题通信）。

使用 `tutorial_interfaces` 中自定义的 `Num` 消息类型。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `talker` | `minimal_publisher` | 定时发布递增的 `Num` 消息 |
| `listener` | `minimal_subscriber` | 订阅 `Num` 消息并打印 |

## 运行

```bash
ros2 run cpp_pubsub talker
# 另一个终端
ros2 run cpp_pubsub listener
```
