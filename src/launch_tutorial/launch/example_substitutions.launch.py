from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, TimerAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node

def generate_launch_description():
    # 定义 substitution（变量），后面引用它们
    turtlesim_ns = LaunchConfiguration('turtlesim_ns')
    use_provided_red = LaunchConfiguration('use_provided_red')
    new_background_r = LaunchConfiguration('new_background_r')

    return LaunchDescription([
        # ① 声明 3 个 launch 参数，带默认值
        DeclareLaunchArgument('turtlesim_ns', default_value='turtlesim1'),
        DeclareLaunchArgument('use_provided_red', default_value='False'),
        DeclareLaunchArgument('new_background_r', default_value='200'),

        # ② 启动 turtlesim_node，namespace 由参数决定
        Node(
            package='turtlesim',
            namespace=turtlesim_ns,          # ← 这里用了变量！
            executable='turtlesim_node',
            name='sim',
        ),

        # ③ 生成第二只乌龟（通过命令行调用服务）
        ExecuteProcess(
            cmd=[['ros2 service call ', turtlesim_ns,
                  '/spawn turtlesim/srv/Spawn ',
                  '"{x: 2, y: 2, theta: 0.2}"']],
            shell=True,
        ),

        # ④ 把背景红色值改成 120（紫色）
        ExecuteProcess(
            cmd=[['ros2 param set ', turtlesim_ns,
                  '/sim background_r 120']],
            shell=True,
        ),

        # ⑤ 2 秒后，如果用 provided red = True，改成 new_background_r
        TimerAction(
            period=2.0,
            actions=[
                ExecuteProcess(
                    condition=IfCondition(
                        PythonExpression([
                            new_background_r, ' == 200',
                            ' and ', use_provided_red
                        ])
                    ),
                    cmd=[['ros2 param set ', turtlesim_ns,
                          '/sim background_r ', new_background_r]],
                    shell=True,
                ),
            ],
        ),
    ])
