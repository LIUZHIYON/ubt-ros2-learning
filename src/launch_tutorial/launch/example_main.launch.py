from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    colors = {'background_r': '200'}

    return LaunchDescription([
        IncludeLaunchDescription(
            # 找到 launch_tutorial 包的路径，拼接 launch/example_substitutions.launch.py
            PathJoinSubstitution([
                FindPackageShare('launch_tutorial'),
                'launch',
                'example_substitutions.launch.py',
            ]),
            # 传给子 launch 的参数
            launch_arguments={
                'turtlesim_ns': 'turtlesim2',
                'use_provided_red': 'True',
                'new_background_r': colors['background_r'],
            }.items()
        ),
    ])
