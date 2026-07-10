#!/bin/bash
# 一键运行 Marker Basic Shapes 演示
# 使用方法: bash run_marker_demo.sh

# 1. 设置环境
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash

# 2. 启动 basic_shapes 节点 (后台)
echo "Starting basic_shapes node..."
ros2 run visualization_marker_tutorials basic_shapes &
NODE_PID=$!

# 短暂等待节点启动
sleep 1

# 3. 打开 RViz2 (前台)
echo "Starting RViz2 with pre-configured marker display..."
echo "  - Fixed Frame = my_frame"
echo "  - Topic = /visualization_marker"
echo "  - Shape cycles: CUBE → SPHERE → ARROW → CYLINDER → CUBE"
echo ""
echo "Press Ctrl+C to stop both."
QT_QPA_PLATFORM=xcb rviz2 -d ~/.rviz2/default.rviz

# 4. 清理
echo "Stopping basic_shapes node..."
kill $NODE_PID 2>/dev/null
wait $NODE_PID 2>/dev/null
echo "Done."
