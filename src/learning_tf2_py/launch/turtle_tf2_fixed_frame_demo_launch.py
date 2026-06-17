from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    return LaunchDescription([
        # 包含之前写的 launch（启动 turtlesim + broadcaster1 + broadcaster2 + listener）
        IncludeLaunchDescription(
            PathJoinSubstitution([
                FindPackageShare('learning_tf2_py'),
                'launch',
                'turtle_tf2_demo_launch.py',
            ]),
            # ★ 告诉 listener 去追 carrot1 而不是 turtle1
            launch_arguments={'target_frame': 'carrot1'}.items(),
        ),
        # 运行固定帧广播器
        Node(
            package='learning_tf2_py',
            executable='fixed_frame_tf2_broadcaster',
            name='fixed_broadcaster',
        ),
    ])
