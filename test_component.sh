#!/bin/bash
source /opt/ros/humble/setup.bash
source /home/ubt/ros2_ws/install/setup.bash

# 启动容器
ros2 run rclcpp_components component_container &
CONTAINER_PID=$!
sleep 2

echo "=== 加载组件 ==="
ros2 component load /ComponentManager composition_demo composition_demo::Talker
sleep 1

echo "=== 节点列表 ==="
ros2 node list

echo "=== 话题列表 ==="
ros2 topic list

echo "=== 清理 ==="
kill $CONTAINER_PID 2>/dev/null
wait $CONTAINER_PID 2>/dev/null

echo "=== 完成 ==="
