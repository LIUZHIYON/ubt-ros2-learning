#!/usr/bin/env python3
"""
Launch file: 一键启动 Gazebo + spawn R2D2 机器人
用法:
  ros2 launch ~/ros2_ws/src/my_robot_urdf/launch/gazebo.launch.py

或者从终端手动两步：
  终端1: ros2 launch ros_gz_sim gz_sim.launch.py gz_args:="empty.sdf"
  终端2: xacro 09-gazebo.urdf.xacro > /tmp/r2d2.urdf && ros2 run ros_gz_sim create -world empty -file /tmp/r2d2.urdf -name r2d2 -z 1.0
"""

import os
import subprocess
import tempfile
from launch import LaunchDescription
from launch.actions import ExecuteProcess, TimerAction, LogInfo
from launch.substitutions import Command
from launch_ros.actions import Node


def generate_launch_description():

    xacro_file = os.path.expanduser(
        '~/ros2_ws/src/my_robot_urdf/urdf/09-gazebo.urdf.xacro'
    )
    urdf_tmp = '/tmp/r2d2.urdf'

    # 预先把 xacro 转成 urdf，存到 /tmp/r2d2.urdf
    subprocess.run(
        ['xacro', xacro_file, '-o', urdf_tmp],
        check=True, capture_output=True
    )

    # URDF 字符串（给 robot_state_publisher）
    robot_description = Command(['xacro ', xacro_file])

    return LaunchDescription([

        # ========== 1. 启动 Gazebo 空世界 ==========
        ExecuteProcess(
            cmd=['gz', 'sim', 'empty.sdf'],
            output='screen',
            name='gazebo'
        ),

        # ========== 2. 启动 robot_state_publisher ==========
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': robot_description}]
        ),

        # ========== 3. ros_gz_bridge：关节状态桥接 ==========
        Node(
            package='ros_gz_bridge',
            executable='parameter_bridge',
            name='bridge_joint_states',
            arguments=[
                '/joint_states@sensor_msgs/msg/JointState@ignition.msgs.Model',
            ],
            output='screen',
        ),

        # ========== 4. spawn URDF 到 Gazebo（延迟 3 秒等 Gazebo 就绪）==========
        TimerAction(
            period=3.0,
            actions=[
                LogInfo(msg='Spawning R2D2 into Gazebo...'),
                ExecuteProcess(
                    cmd=[
                        'ros2', 'run', 'ros_gz_sim', 'create',
                        '-world', 'empty',
                        '-file', urdf_tmp,       # ← 用转换好的 .urdf 文件！
                        '-name', 'r2d2',
                        '-z', '1.0',
                    ],
                    output='screen',
                    name='spawn_r2d2'
                ),
            ]
        ),

    ])
