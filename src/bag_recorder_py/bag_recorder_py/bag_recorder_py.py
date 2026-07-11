"""
rosbag2 Python API 演示 —— 从节点内部录制话题

功能：
  1. 创建 publisher 发布消息
  2. 同时用 rosbag2_py 写到 sqlite3 bag 文件
  3. Ctrl-C 停止，bag 自动保存

运行：         ros2 run bag_recorder_py bag_recorder_py
回放：         ros2 bag play my_bag_py/
查看 bag 信息： ros2 bag info my_bag_py/
"""

import rclpy
from rclpy.node import Node
from rclpy.serialization import serialize_message
from std_msgs.msg import String

# rosbag2 Python API
from rosbag2_py import SequentialWriter, StorageOptions, ConverterOptions
from rosbag2_py import TopicMetadata


class BagRecorderPy(Node):
    def __init__(self):
        super().__init__('bag_recorder_py')
        self.count = 0

        # ---- 1. 创建 publisher ----
        self.publisher = self.create_publisher(String, 'bag_demo_py_topic', 10)

        # ---- 2. 创建并打开 rosbag2 Writer ----
        self.writer = SequentialWriter()

        storage_options = StorageOptions(
            uri='my_bag_py',
            storage_id='sqlite3',
        )
        self.writer.open(storage_options, ConverterOptions())

        # 注册 topic（必须先注册才能写）
        topic_info = TopicMetadata(
            name='bag_demo_py_topic',
            type='std_msgs/msg/String',
            serialization_format='cdr',
        )
        self.writer.create_topic(topic_info)

        self.get_logger().info(f'📦 开始录制 -> {storage_options.uri}')

        # ---- 3. 定时器：每 500ms 发布并录制 ----
        self.timer = self.create_timer(0.5, self.timer_callback)

    def timer_callback(self):
        msg = String()
        msg.data = f'Hello from Python bag recorder #{self.count}'

        self.get_logger().info(f"Recording: '{msg.data}'")

        # 写入 bag（需要手动序列化消息）
        serialized = serialize_message(msg)
        # write(topic_name, serialized_bytes, timestamp_nanoseconds)
        timestamp = self.get_clock().now().nanoseconds
        self.writer.write('bag_demo_py_topic', serialized, timestamp)

        self.publisher.publish(msg)
        self.count += 1

    def destroy_node(self):
        self.get_logger().info(f'📦 录制停止，共 {self.count} 条消息')
        super().destroy_node()


def main():
    rclpy.init()

    print('\n'
          '========================================\n'
          '  rosbag2 Python API 录制演示\n'
          '========================================\n'
          '  每 500ms 发布并录制到 my_bag_py/\n'
          '  Ctrl-C 停止，bag 自动保存\n'
          '========================================\n')

    node = BagRecorderPy()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
