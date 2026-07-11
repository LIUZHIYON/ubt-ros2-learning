"""
rqt_bag 自定义插件：按诊断状态着色
"""
from python_qt_binding.QtCore import Qt
from python_qt_binding.QtGui import QColor, QPainter, QPen

from rclpy.serialization import deserialize_message
from rclpy.time import Time
from diagnostic_msgs.msg import DiagnosticStatus

from rqt_bag.plugins.plugin import Plugin


# 诊断等级 → 颜色映射
LEVEL_COLORS = {
    DiagnosticStatus.OK:    QColor(0, 255, 0),      # 绿色
    DiagnosticStatus.WARN:  QColor(255, 255, 0),    # 黄色
    DiagnosticStatus.ERROR: QColor(255, 0, 0),      # 红色
    DiagnosticStatus.STALE: QColor(128, 128, 128),  # 灰色
}

LEVEL_NAMES = {
    DiagnosticStatus.OK:    'OK',
    DiagnosticStatus.WARN:  'WARN',
    DiagnosticStatus.ERROR: 'ERROR',
    DiagnosticStatus.STALE: 'STALE',
}


class DiagnosticBagPlugin(Plugin):
    """
    自定义 rqt_bag 插件：在时间线上按诊断等级画不同颜色
    """

    def __init__(self):
        super().__init__()
        # 告诉 rqt_bag 订阅哪些 topic
        self.set_topic('/diagnostics')

    def draw_timeline_segment(
        self,
        painter: QPainter,
        topic: str,
        start: float,
        end: float,
        x: float,
        y: int,
        width: float,
        height: int,
    ):
        """
        在时间线的一段区间上画消息
        被 rqt_bag 自动调用
        """
        bag_timeline = self.timeline.scene()
        start_t = Time(seconds=start)
        end_t = Time(seconds=end)

        for bag, entry in bag_timeline.get_entries_with_bags(
            [topic], start_t, end_t
        ):
            topic_name, raw_data, t = bag_timeline.read_message(bag, entry)

            # 反序列化消息
            msg = deserialize_message(raw_data, DiagnosticStatus)

            # 根据等级选颜色
            level = msg.level
            color = LEVEL_COLORS.get(level, QColor(0, 0, 0))

            # 计算时间位置
            msg_x = bag_timeline.map_timestamp_to_x(t) + x
            msg_width = max(width, 8)  # 至少 8 像素宽

            # 画色块
            painter.fillRect(
                int(msg_x), y, int(msg_width), height, color
            )

    def get_message_display_name(self, msg, topic):
        """在话题列表里显示带颜色的状态名"""
        level = msg.level
        name = LEVEL_NAMES.get(level, f'UNKNOWN({level})')
        return name
