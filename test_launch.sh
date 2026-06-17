#!/bin/bash
source /opt/ros/humble/setup.bash
source /home/ubt/ros2_ws/install/setup.bash
pkill -f component_container 2>/dev/null
sleep 1

echo "=== 一键启动：ros2 launch ==="
ros2 launch composition_demo composition_demo.launch.py &
LAUNCH_PID=$!
sleep 4

echo ""
echo "=== 节点列表 ==="
ros2 node list 2>/dev/null | grep -v WARNING | sort -u

echo ""
echo "=== 进程（只有1个容器）==="
ps aux | grep '[c]omponent_container_mt\|[c]omponent_container ' | awk '{print "PID:", $2, "CMD:", $11}'

echo ""
echo "=== 消息收发（观察3秒）==="
sleep 3

echo ""
echo "=== 停止 ==="
kill $LAUNCH_PID 2>/dev/null
wait $LAUNCH_PID 2>/dev/null
echo "✅ 完成"
