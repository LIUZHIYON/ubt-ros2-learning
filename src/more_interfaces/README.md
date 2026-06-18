# more_interfaces

自定义 ROS 2 接口 + 使用示例——通讯录（AddressBook）。

## 接口

- **消息：** `AddressBook.msg` — 包含个人信息和联系人列表

## 节点

| 可执行文件 | 节点名 | 说明 |
|---|---|---|
| `publish_address_book` | `address_book_publisher` | 发布一条 AddressBook 消息 |

## 运行

```bash
ros2 run more_interfaces publish_address_book
```
