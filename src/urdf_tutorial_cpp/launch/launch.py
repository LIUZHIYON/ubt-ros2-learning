from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
import os


def generate_launch_description():

    use_sim_time = LaunchConfiguration('use_sim_time', default='false')

    urdf_file = os.path.join(
        get_package_share_directory('urdf_tutorial_cpp'),
        'urdf',
        'r2d2.urdf.xml'
    )

    with open(urdf_file, 'r') as f:
        robot_desc = f.read()

    # ── RViz 配置文件 ──
    rviz_config = os.path.join(
        get_package_share_directory('urdf_tutorial_cpp'),
        'urdf',
        'r2d2.rviz'
    )

    return LaunchDescription([

        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use simulation (Gazebo) clock if true'
        ),

        # 1. C++ 节点：发布 /joint_states + TF(odom→axis)
        Node(
            package='urdf_tutorial_cpp',
            executable='urdf_tutorial',
            name='state_publisher',
            output='screen',
            parameters=[{'use_sim_time': use_sim_time}]
        ),

        # 2. robot_state_publisher：URDF → TF 变换
        #     publish_robot_description=True 把 URDF 发布到 /robot_description 话题，
        #     RViz 订阅该话题即可显示机器人模型
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{
                'use_sim_time': use_sim_time,
                'robot_description': robot_desc,
                'publish_robot_description': True
            }]
        ),

        # 3. RViz2（★关键：也传 robot_description 参数给它！）
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config],
            parameters=[{
                'use_sim_time': use_sim_time,
                'robot_description': robot_desc   # ← RViz 也需要
            }]
        ),

    ])
