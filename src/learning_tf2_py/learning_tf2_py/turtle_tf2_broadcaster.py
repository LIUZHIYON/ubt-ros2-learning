import math

from geometry_msgs.msg import TransformStamped
import numpy as np
import rclpy
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
from turtlesim.msg import Pose


# 欧拉角 → 四元数（跟上一个教程一样）
def quaternion_from_euler(ai, aj, ak):
    ai /= 2.0
    aj /= 2.0
    ak /= 2.0
    ci = math.cos(ai)
    si = math.sin(ai)
    cj = math.cos(aj)
    sj = math.sin(aj)
    ck = math.cos(ak)
    sk = math.sin(ak)
    cc = ci * ck
    cs = ci * sk
    sc = si * ck
    ss = si * sk
    q = np.empty((4,))
    q[0] = cj * sc - sj * cs
    q[1] = cj * ss + sj * cc
    q[2] = cj * cs - sj * sc
    q[3] = cj * cc + sj * ss
    return q


class FramePublisher(Node):

    def __init__(self):
        super().__init__('turtle_tf2_frame_publisher')

        # ★ 1. 声明一个参数：海龟的名字（turtle1 或 turtle2）
        self.turtlename = self.declare_parameter(
            'turtlename', 'turtle'
        ).get_parameter_value().string_value

        # ★ 2. 创建动态变换广播器（不是静态！）
        self.tf_broadcaster = TransformBroadcaster(self)

        # ★ 3. 订阅 /turtleX/pose 话题
        #     每次收到位置数据就调用 handle_turtle_pose
        self.subscription = self.create_subscription(
            Pose,
            f'/{self.turtlename}/pose',
            self.handle_turtle_pose,
            1)
        self.subscription  # 防止"未使用变量"警告

    def handle_turtle_pose(self, msg):
        # 构造 TransformStamped 消息
        t = TransformStamped()

        # 时间戳
        t.header.stamp = self.get_clock().now().to_msg()
        # 父坐标系 = world
        t.header.frame_id = 'world'
        # 子坐标系 = turtle1（或 turtle2）
        t.child_frame_id = self.turtlename

        # 位置：海龟在 2D 平面，z=0
        t.transform.translation.x = msg.x
        t.transform.translation.y = msg.y
        t.transform.translation.z = 0.0

        # 旋转：海龟只能绕 Z 轴转，所以 roll=0, pitch=0
        q = quaternion_from_euler(0, 0, msg.theta)
        t.transform.rotation.x = q[0]
        t.transform.rotation.y = q[1]
        t.transform.rotation.z = q[2]
        t.transform.rotation.w = q[3]

        # 发布到 TF2
        self.tf_broadcaster.sendTransform(t)


def main():
    rclpy.init()
    node = FramePublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    rclpy.shutdown()
