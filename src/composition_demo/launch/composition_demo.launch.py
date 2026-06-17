"""
composition_demo.launch.py

作用：一键启动容器 + 自动加载 Talker 和 Listener 两个组件
等价于你手动敲：
  1. ros2 run rclcpp_components component_container
  2. ros2 component load /ComponentManager composition_demo composition_demo::Talker
  3. ros2 component load /ComponentManager composition_demo composition_demo::Listener
"""

# ==================== 导入 ====================

from launch import LaunchDescription
# LaunchDescription 是整个 launch 文件的"总描述"，return 给它就行

from launch_ros.actions import ComposableNodeContainer
# ComposableNodeContainer = 定义"容器进程"
# 它就是 component_container 在 launch 里的写法

from launch_ros.descriptions import ComposableNode
# ComposableNode = 描述"一个要装入容器的组件"
# 一个 ComposableNode 对应终端里的一次 ros2 component load

# ==================== 主函数 ====================

def generate_launch_description():
    """
    ros2 launch 固定会调这个函数。
    函数名必须是 generate_launch_description，不能改。
    返回一个 LaunchDescription 对象。
    """

    # ===== 定义容器 =====
    # 类比：ros2 run rclcpp_components component_container
    container = ComposableNodeContainer(

        # 容器在 ROS 图里的节点名（用 ros2 node list 能看到）
        name='my_container',

        # 命名空间，空字符串 = 不用命名空间
        # 如果填 'robot1'，那么节点的完整名字是 /robot1/my_container
        namespace='',

        # 容器程序来自哪个包
        package='rclcpp_components',

        # 用哪个可执行文件做容器
        # component_container        → 单线程容器
        # component_container_mt     → 多线程容器（组件多时用）
        # component_container_isolated → 隔离开的容器
        executable='component_container',

        # ===== 定义要装进容器的组件列表 =====
        composable_node_descriptions=[

            # ----- Talker 组件 -----
            # 类比：ros2 component load /ComponentManager composition_demo composition_demo::Talker
            ComposableNode(
                # 组件在哪个包里
                package='composition_demo',

                # ★ 组件类的全名（命名空间::类名）
                # 必须和 C++ 代码里 RCLCPP_COMPONENTS_REGISTER_NODE 注册的类名一致
                # 必须和 CMakeLists.txt 里 PLUGIN 填的字符串一致
                plugin='composition_demo::Talker',

                # 组件在 ROS 图里的节点名（用 ros2 node list 能看到）
                name='talker',

                # 可以在这里传参数，比如：
                # parameters=[{'publish_rate': 500}],
            ),

            # ----- Listener 组件 -----
            ComposableNode(
                package='composition_demo',
                plugin='composition_demo::Listener',
                name='listener',
            ),

            # 想加更多组件？继续加 ComposableNode(...) 就行
        ],

        # 容器的输出打到哪
        # 'screen' → 打印到终端（调试时用）
        # 'log'    → 只写日志文件
        output='screen',
    )

    # ===== 返回 =====
    # LaunchDescription 里放所有要启动的东西
    # 这里只有一个 container，但里面装了 talker + listener 两个组件
    return LaunchDescription([container])
