import rclpy
from rclpy.action import ActionClient      # ← Action 客户端
from rclpy.node import Node
from action_tutorials_interfaces.action import Fibonacci

class FibonacciActionClient(Node):
    def __init__(self):
        super().__init__('fibonacci_action_client')
        # ★ 创建 Action 客户端
        # 参数1：节点
        # 参数2：Action 类型
        # 参数3：Action 名字（必须和服务端一致）
        self._action_client = ActionClient(self, Fibonacci, 'fibonacci')

    def send_goal(self, order):
        # 创建目标消息
        goal_msg = Fibonacci.Goal()
        goal_msg.order = order

        # 等待服务端上线
        self._action_client.wait_for_server()

        # ★ 发送目标，并注册回调
        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg,
            feedback_callback=self.feedback_callback)  # ← 注册反馈回调
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    # ★ 服务端接受/拒绝目标后的回调
    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('Goal rejected :(')
            return
        self.get_logger().info('Goal accepted :)')

        # 请求最终结果
        self._get_result_future = goal_handle.get_result_async()
        self._get_result_future.add_done_callback(self.get_result_callback)

    # ★ 收到最终结果时的回调
    def get_result_callback(self, future):
        result = future.result().result
        self.get_logger().info('Result: {0}'.format(result.sequence))
        rclpy.shutdown()

    # ★ 收到进度反馈时的回调
    def feedback_callback(self, feedback_msg):
        feedback = feedback_msg.feedback
        self.get_logger().info('Received feedback: {0}'.format(feedback.partial_sequence))

def main(args=None):
    rclpy.init(args=args)
    action_client = FibonacciActionClient()
    action_client.send_goal(10)  # ★ 请求计算前 10 个斐波那契数
    rclpy.spin(action_client)

if __name__ == '__main__':
    main()
