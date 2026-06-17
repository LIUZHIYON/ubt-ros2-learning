from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        # 1. turtlesim 窗口
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='sim',
        ),
        # 2. turtle1 的 TF 广播器
        Node(
            package='learning_tf2_py',
            executable='turtle_tf2_broadcaster',
            name='broadcaster1',
            parameters=[{'turtlename': 'turtle1'}],
        ),
        # 3. launch 参数：目标坐标系（默认 turtle1）
        DeclareLaunchArgument(
            'target_frame',
            default_value='turtle1',
            description='Target frame name.',
        ),
        # 4. turtle2 的 TF 广播器
        Node(
            package='learning_tf2_py',
            executable='turtle_tf2_broadcaster',
            name='broadcaster2',
            parameters=[{'turtlename': 'turtle2'}],
        ),
        # 5. TF 监听器（自动让 turtle2 跟 turtle1）
        Node(
            package='learning_tf2_py',
            executable='turtle_tf2_listener',
            name='listener',
            parameters=[{
                'target_frame': LaunchConfiguration('target_frame')
            }],
        ),
    ])
