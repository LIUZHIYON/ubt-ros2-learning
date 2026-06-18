# tutorial_interfaces

自定义 ROS 2 消息和服务接口包。

## 接口定义

| 类型 | 文件名 | 字段 |
|---|---|---|
| 消息 | `Num.msg` | `int64 num` |
| 消息 | `Sphere.msg` | `geometry_msgs/Point center` + `float64 radius` |
| 服务 | `AddThreeInts.srv` | Request: `int64 a, int64 b, int64 c` / Response: `int64 sum` |

## 构建

```bash
colcon build --packages-select tutorial_interfaces
source install/setup.bash
```

## 使用

这是一个纯接口包，被 `cpp_pubsub`、`py_srvcli` 等包依赖使用。
