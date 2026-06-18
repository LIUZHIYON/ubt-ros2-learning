# py_srvcli

ROS 2 服务/客户端 Python 基础示例，使用 `tutorial_interfaces` 中自定义的 `AddThreeInts` 服务。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `service` | `minimal_service` | 服务端，接收三个 int 并返回和 |
| `client` | `minimal_client` | 客户端，发送三个 int 请求并打印结果 |

## 运行

```bash
ros2 run py_srvcli service
# 另一个终端
ros2 run py_srvcli client 2 3 1
```
