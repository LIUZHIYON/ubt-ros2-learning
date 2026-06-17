#!/bin/bash
source /opt/ros/humble/setup.bash
source /home/ubt/ros2_ws/install/setup.bash

echo "=========================================="
echo "  组合节点 Demo：Talker + Listener 同进程"
echo "=========================================="
echo ""

# 启动容器（后台）
ros2 run rclcpp_components component_container &
CONTAINER_PID=$!
sleep 2

echo "📦 容器已启动（PID: $CONTAINER_PID）"
echo ""

# 查看容器里目前有几个节点
echo "=== 加载前：ros2 node list ==="
ros2 node list
echo ""

# 加载 Talker 组件
echo ">>> 加载 Talker 组件..."
ros2 component load /ComponentManager composition_demo composition_demo::Talker
sleep 1

# 加载 Listener 组件
echo ""
echo ">>> 加载 Listener 组件..."
ros2 component load /ComponentManager composition_demo composition_demo::Listener
sleep 1

# 现在查看所有节点
echo ""
echo "==========================================
🎯 关键：两个节点在同一个进程里！
=========================================="
echo ""
echo "=== 加载后：ros2 node list ==="
ros2 node list
echo ""

# 看进程 —— 只有一个 component_container 进程
echo "=== 系统进程 ==="
ps aux | grep component_container | grep -v grep
echo ""

echo "=========================================="
echo "  📡 消息收发中...（5秒后自动停止）"
echo "=========================================="
sleep 5

kill $CONTAINER_PID 2>/dev/null
wait $CONTAINER_PID 2>/dev/null
echo ""
echo "✅ Demo 结束"
