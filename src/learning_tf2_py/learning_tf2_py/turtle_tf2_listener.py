import math

from geometry_msgs.msg import Twist
import rclpy
from rclpy.node import Node
from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
from turtlesim.srv import Spawn


class FrameListener(Node):

    def __init__(self):
        super().__init__('turtle_tf2_frame_listener')

        # ★ 1. 声明参数：要追踪的目标坐标系
        self.target_frame = self.declare_parameter(
            'target_frame', 'turtle1'
        ).get_parameter_value().string_value

        # ★ 2. 创建 TF 缓存和监听器
        #     Buffer 存储收到的所有 TF 数据，最长保留 10 秒
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # ★ 3. 创建 spawn 服务客户端（用来生成第二只海龟）
        self.spawner = self.create_client(Spawn, 'spawn')

        self.turtle_spawning_service_ready = False
        self.turtle_spawned = False

        # ★ 4. 创建发布者：发布速度指令到 turtle2
        self.publisher = self.create_publisher(Twist, 'turtle2/cmd_vel', 1)

        # ★ 5. 每秒调用一次 on_timer
        self.timer = self.create_timer(1.0, self.on_timer)

    def on_timer(self):
        from_frame_rel = self.target_frame   # 目标：turtle1
        to_frame_rel = 'turtle2'            # 自己：turtle2

        # ── 如果 spawn 服务已就绪且海龟已生成 ──
        if self.turtle_spawning_service_ready:
            if self.turtle_spawned:
                # ★ 核心：查询 turtle2 → turtle1 的变换
                try:
                    t = self.tf_buffer.lookup_transform(
                        to_frame_rel,       # 目标坐标系
                        from_frame_rel,     # 源坐标系
                        rclpy.time.Time())  # 获取最新变换
                except TransformException as ex:
                    self.get_logger().info(
                        f'查询失败 {to_frame_rel} → {from_frame_rel}: {ex}')
                    return

                # ★ 根据变换计算速度指令
                msg = Twist()

                # 角速度：朝向目标
                msg.angular.z = 1.0 * math.atan2(
                    t.transform.translation.y,
                    t.transform.translation.x)

                # 线速度：向目标前进
                msg.linear.x = 0.5 * math.sqrt(
                    t.transform.translation.x ** 2 +
                    t.transform.translation.y ** 2)

                # 发布速度指令 → turtle2 就会移动
                self.publisher.publish(msg)
            else:
                # 等待 spawn 完成
                if self.result.done():
                    self.get_logger().info(
                        f'成功生成 {self.result.result().name}')
                    self.turtle_spawned = True
                else:
                    self.get_logger().info('Spawn 还未完成')
        else:
            # 第一次调用：检查 spawn 服务是否就绪
            if self.spawner.service_is_ready():
                # 调用 spawn 服务生成 turtle2
                request = Spawn.Request()
                request.name = 'turtle2'
                request.x = float(4)
                request.y = float(2)
                request.theta = float(0)
                self.result = self.spawner.call_async(request)
                self.turtle_spawning_service_ready = True
            else:
                self.get_logger().info('Spawn 服务未就绪')


def main():
    rclpy.init()
    node = FrameListener()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    rclpy.shutdown()
