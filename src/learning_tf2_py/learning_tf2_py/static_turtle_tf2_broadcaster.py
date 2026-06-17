import math
import sys

from geometry_msgs.msg import TransformStamped
import numpy as np
import rclpy
from rclpy.node import Node
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


# 把欧拉角（roll, pitch, yaw）转成四元数（ROS 用四元数表示旋转）
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


class StaticFramePublisher(Node):
    """
    发布一个永远不会改变的变换。
    从 world → 你指定的静态坐标系，只在启动时发布一次。
    """

    def __init__(self, transformation):
        super().__init__('static_turtle_tf2_broadcaster')
        # 创建静态变换广播器
        self.tf_static_broadcaster = StaticTransformBroadcaster(self)
        # 启动时发布一次
        self.make_transforms(transformation)

    def make_transforms(self, transformation):
        t = TransformStamped()

        # ① 时间戳
        t.header.stamp = self.get_clock().now().to_msg()
        # ② 父坐标系
        t.header.frame_id = 'world'
        # ③ 子坐标系（从命令行参数获取）
        t.child_frame_id = transformation[1]

        # ④ 位置（x, y, z）
        t.transform.translation.x = float(transformation[2])
        t.transform.translation.y = float(transformation[3])
        t.transform.translation.z = float(transformation[4])

        # ⑤ 旋转（欧拉角 → 四元数）
        quat = quaternion_from_euler(
            float(transformation[5]),  # roll
            float(transformation[6]),  # pitch
            float(transformation[7]))  # yaw
        t.transform.rotation.x = quat[0]
        t.transform.rotation.y = quat[1]
        t.transform.rotation.z = quat[2]
        t.transform.rotation.w = quat[3]

        # ⑥ 发布静态变换
        self.tf_static_broadcaster.sendTransform(t)


def main():
    logger = rclpy.logging.get_logger('logger')

    # 检查命令行参数：共需要 7 个参数
    if len(sys.argv) != 8:
        logger.info(
            '参数数量不对。用法：\n'
            '$ ros2 run learning_tf2_py static_turtle_tf2_broadcaster '
            'child_frame_name x y z roll pitch yaw')
        sys.exit(1)

    # 子坐标系不能叫 world
    if sys.argv[1] == 'world':
        logger.info('子坐标系名不能是 "world"')
        sys.exit(2)

    # 初始化节点
    rclpy.init()
    node = StaticFramePublisher(sys.argv)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    rclpy.shutdown()


if __name__ == '__main__':
    main()
