# node_interfaces_tutorial

ROS 2 节点接口教程——展示如何直接使用 `NodeInterfaces`。

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `node_interfaces_example` | `node_interfaces_example` | 传入各个接口（Parameters / Topics / Logging 等）构造节点 |
| `node_interfaces_template_example` | `node_interfaces_template` | 模板化版本，支持不同接口组合 |

## 运行

```bash
ros2 run node_interfaces_tutorial node_interfaces_example
ros2 run node_interfaces_tutorial node_interfaces_template_example
```
