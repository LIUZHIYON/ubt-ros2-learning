from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    EmitEvent,
    ExecuteProcess,
    LogInfo,
    RegisterEventHandler,
    TimerAction,
)
from launch.conditions import IfCondition
from launch.event_handlers import (
    OnExecutionComplete,
    OnProcessExit,
    OnProcessIO,
    OnProcessStart,
    OnShutdown,
)
from launch.events import Shutdown
from launch.substitutions import (
    EnvironmentVariable,
    FindExecutable,
    LaunchConfiguration,
    LocalSubstitution,
    PythonExpression,
)
from launch_ros.actions import Node


def generate_launch_description():
    # ── 定义 3 个 launch 参数 ──
    turtlesim_ns = LaunchConfiguration('turtlesim_ns')
    use_provided_red = LaunchConfiguration('use_provided_red')
    new_background_r = LaunchConfiguration('new_background_r')

    turtlesim_ns_launch_arg = DeclareLaunchArgument(
        'turtlesim_ns', default_value='turtlesim1')
    use_provided_red_launch_arg = DeclareLaunchArgument(
        'use_provided_red', default_value='False')
    new_background_r_launch_arg = DeclareLaunchArgument(
        'new_background_r', default_value='200')

    # ── 启动海龟节点 ──
    turtlesim_node = Node(
        package='turtlesim',
        namespace=turtlesim_ns,
        executable='turtlesim_node',
        name='sim',
    )

    # ── spawn 第二只海龟 ──
    spawn_turtle = ExecuteProcess(
        cmd=[[
            FindExecutable(name='ros2'),
            ' service call ',
            turtlesim_ns,
            '/spawn turtlesim/srv/Spawn ',
            '"{x: 2, y: 2, theta: 0.2}"',
        ]],
        shell=True,
    )

    # ── 改成紫色背景 ──
    change_background_r = ExecuteProcess(
        cmd=[[
            FindExecutable(name='ros2'),
            ' param set ',
            turtlesim_ns,
            '/sim background_r 120',
        ]],
        shell=True,
    )

    # ── 改成粉色背景（有条件） ──
    change_background_r_conditioned = ExecuteProcess(
        condition=IfCondition(
            PythonExpression([
                new_background_r, ' == 200',
                ' and ', use_provided_red,
            ])
        ),
        cmd=[[
            FindExecutable(name='ros2'),
            ' param set ',
            turtlesim_ns,
            '/sim background_r ',
            new_background_r,
        ]],
        shell=True,
    )

    return LaunchDescription([
        # ── launch 参数 ──
        turtlesim_ns_launch_arg,
        use_provided_red_launch_arg,
        new_background_r_launch_arg,

        # ── 启动节点 ──
        turtlesim_node,

        # ─── ★★★ 事件处理器从这里开始 ★★★ ───

        # 事件 1：turtlesim 启动后 → spawn 第二只龟
        RegisterEventHandler(
            OnProcessStart(
                target_action=turtlesim_node,
                on_start=[
                    LogInfo(msg='Turtlesim 启动完成，开始 spawn 海龟'),
                    spawn_turtle,
                ]
            )
        ),

        # 事件 2：spawn 命令有输出时 → 打印输出内容
        RegisterEventHandler(
            OnProcessIO(
                target_action=spawn_turtle,
                on_stdout=lambda event: LogInfo(
                    msg='Spawn 返回: "{}"'.format(
                        event.text.decode().strip())
                )
            )
        ),

        # 事件 3：spawn 执行完成后 → 变色
        RegisterEventHandler(
            OnExecutionComplete(
                target_action=spawn_turtle,
                on_completion=[
                    LogInfo(msg='Spawn 完成'),
                    change_background_r,
                    TimerAction(
                        period=2.0,
                        actions=[change_background_r_conditioned],
                    ),
                ]
            )
        ),

        # 事件 4：turtlesim 退出时 → 打印谁关的 + 关闭 launch
        RegisterEventHandler(
            OnProcessExit(
                target_action=turtlesim_node,
                on_exit=[
                    LogInfo(msg=(
                        EnvironmentVariable(name='USER'),
                        ' 关闭了 turtlesim 窗口',
                    )),
                    EmitEvent(event=Shutdown(reason='窗口已关闭')),
                ]
            )
        ),

        # 事件 5：launch 被要求关闭时 → 打印原因
        RegisterEventHandler(
            OnShutdown(
                on_shutdown=[
                    LogInfo(msg=[
                        'Launch 被要求关闭，原因：',
                        LocalSubstitution('event.reason'),
                    ])
                ]
            )
        ),
    ])
