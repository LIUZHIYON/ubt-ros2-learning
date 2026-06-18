# composition_demo

ROS 2 组件组合（Composition）演示，将发布者和订阅者封装为可动态加载的组件。

## 节点组件

| 可执行文件 / 组件 | 类名 | 说明 |
|---|---|---|
| `talker_component` | `composition_demo::Talker` | 定时发布 `std_msgs/msg/String` |
| `listener_component` | `composition_demo::Listener` | 订阅并打印消息 |

## 运行

### 方式 1：独立运行

```bash
# 终端 1
ros2 run composition_demo talker_component
# 终端 2
ros2 run composition_demo listener_component
```

### 方式 2：通过 launch 文件组合

```bash
ros2 launch composition_demo composition_demo.launch.py
```

### 方式 3：手动组合（component container）

```bash
# 启动容器
ros2 run rclcpp_components component_container

# 加载组件
ros2 component load /ComponentManager composition_demo composition_demo::Talker
ros2 component load /ComponentManager composition_demo composition_demo::Listener
```
