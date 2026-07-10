#!/usr/bin/env python3
"""
Launch 文件：启动 robot_state_publisher + joint_state_publisher_gui + RViz2

robot_state_publisher：  读取 URDF，发布 robot_description + 各连杆的 TF 坐标变换
joint_state_publisher_gui：弹出 GUI 滑块窗口，让你拖拽关节角度
static_transform_publisher：发布 odom→base_link 静态 TF（让 RViz 固定帧 = odom 也能工作）
rviz2：                  显示机器人模型，使用预置的 rviz 配置文件
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # ---- Launch 参数 ----
    use_sim_time = LaunchConfiguration('use_sim_time', default='false')
    start_rviz = LaunchConfiguration('rviz', default='true')

    # ---- 找到 URDF 文件路径 ----
    pkg_path = get_package_share_directory('second_robot')
    urdf_path = os.path.join(pkg_path, 'urdf', 'my_robot.urdf')

    # ---- 找到 RViz 配置文件路径 ----
    rviz_config = os.path.join(pkg_path, 'rviz', 'my_robot.rviz')

    # ---- 读取 URDF 文件内容 ----
    with open(urdf_path, 'r') as f:
        robot_desc = f.read()

    # ---- robot_state_publisher 节点 ----
    # 把 URDF 字符串通过 robot_description 参数传入
    # 并显式发布到 /robot_description 话题供 RViz 订阅
    rsp_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_desc,
            'use_sim_time': use_sim_time,
            'publish_robot_description': True,
        }]
    )

    # ---- joint_state_publisher_gui 节点 ----
    # 弹出一个 GUI 窗口，可以拖滑块控制 joint_1 和 joint_2 的角度
    jsp_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        output='screen',
    )

    # ---- static_transform_publisher 节点 ----
    # 发布 odom → base_link 的静态单位变换（无平移、无旋转）
    # 这样 RViz 的 Fixed Frame 设为 "odom" 时也能找到 TF 树
    static_tf_node = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='static_tf_odom_to_base_link',
        arguments=['0', '0', '0', '0', '0', '0', 'odom', 'base_link'],
    )

    # ---- RViz2 节点（可选，通过 rviz 参数控制） ----
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_config],
        parameters=[{
            'use_sim_time': use_sim_time,
            'robot_description': robot_desc,
        }],
        condition=IfCondition(start_rviz),
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use simulation (Gazebo) clock if true'
        ),
        DeclareLaunchArgument(
            'rviz',
            default_value='true',
            description='Launch RViz2 automatically'
        ),
        rsp_node,
        jsp_node,
        static_tf_node,
        rviz_node,
    ])
