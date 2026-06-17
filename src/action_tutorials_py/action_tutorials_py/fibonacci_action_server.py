import time
import rclpy
from rclpy.action import ActionServer      # ← Action 服务端
from rclpy.node import Node
from action_tutorials_interfaces.action import Fibonacci  # ← 上篇定义的自定义 Action

class FibonacciActionServer(Node):
    def __init__(self):
        super().__init__('fibonacci_action_server')
        # ★ 创建 Action 服务端
        # 参数1：节点本身
        # 参数2：Action 类型（Fibonacci）
        # 参数3：Action 名字（客户端通过这个名字找到你）
        # 参数4：收到目标后的执行回调
        self._action_server = ActionServer(
            self,
            Fibonacci,
            'fibonacci',
            self.execute_callback)

    # ★ 这个回调在有客户端发送目标时被调用
    def execute_callback(self, goal_handle):
        self.get_logger().info('Executing goal...')

        # 初始化反馈消息
        feedback_msg = Fibonacci.Feedback()
        feedback_msg.partial_sequence = [0, 1]

        # 计算斐波那契数列
        for i in range(1, goal_handle.request.order):
            feedback_msg.partial_sequence.append(
                feedback_msg.partial_sequence[i] +
                feedback_msg.partial_sequence[i-1])
            # ★ 发布反馈（让客户端知道进度）
            self.get_logger().info('Feedback: {0}'.format(feedback_msg.partial_sequence))
            goal_handle.publish_feedback(feedback_msg)
            time.sleep(1)  # 慢一点，让你能看到反馈

        # ★ 标记目标为成功
        goal_handle.succeed()

        # ★ 返回最终结果
        result = Fibonacci.Result()
        result.sequence = feedback_msg.partial_sequence
        return result

def main(args=None):
    rclpy.init(args=args)
    fibonacci_action_server = FibonacciActionServer()
    rclpy.spin(fibonacci_action_server)

if __name__ == '__main__':
    main()
