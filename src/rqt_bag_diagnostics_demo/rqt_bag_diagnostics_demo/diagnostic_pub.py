#!/usr/bin/env python3
"""
测试数据生成器 —— 随机发布 DiagnosticStatus 消息
录制后用 rqt_bag 打开就可以看到插件效果

用法：
  1. 先录 bag:
     ros2 bag record -o diagnostics_bag /diagnostics

  2. 再开另一个终端运行本脚本：
     ros2 run rqt_bag_diagnostics_demo diagnostic_pub

  3. Ctrl-C 停止录制
  4. ros2 run rqt_bag rqt_bag diagnostics_bag/
"""
import random
import rclpy
from rclpy.node import Node
from diagnostic_msgs.msg import DiagnosticStatus


class DiagnosticPub(Node):
    def __init__(self):
        super().__init__('diagnostic_pub')
        self.publisher = self.create_publisher(DiagnosticStatus, '/diagnostics', 10)
        self.last_status = 0  # 从 OK 开始

        self.timer = self.create_timer(1.0, self.callback)
        self.status_values = [0, 1, 2, 3]  # OK, WARN, ERROR, STALE

    def callback(self):
        # 随机切换状态
        if random.randint(0, 3) == 0:
            self.last_status = random.choice(self.status_values)

        msg = DiagnosticStatus()
        msg.level = bytes([self.last_status])
        msg.name = 'demo_sensor'
        msg.message = f'Current status: {self.last_status}'

        levels = ['OK', 'WARN', 'ERROR', 'STALE']
        self.get_logger().info(f'Publishing: {levels[self.last_status]}')
        self.publisher.publish(msg)


def main():
    rclpy.init()
    node = DiagnosticPub()
    rclpy.spin(node)


if __name__ == '__main__':
    main()
