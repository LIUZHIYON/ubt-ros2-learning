cmake_minimum_required(VERSION 3.8)
project(learning_tf2_cpp)

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(turtlesim REQUIRED)

# ---- 1. 静态广播 ----
add_executable(static_turtle_tf2_broadcaster
  src/static_turtle_tf2_broadcaster.cpp
)
ament_target_dependencies(static_turtle_tf2_broadcaster
  geometry_msgs
  rclcpp
  tf2
  tf2_ros
)

# ---- 2. 动态广播 ----
add_executable(turtle_tf2_broadcaster
  src/turtle_tf2_broadcaster.cpp
)
ament_target_dependencies(turtle_tf2_broadcaster
  geometry_msgs
  rclcpp
  tf2
  tf2_ros
  turtlesim
)

# ---- 3. 监听器 ----
add_executable(turtle_tf2_listener
  src/turtle_tf2_listener.cpp
)
ament_target_dependencies(turtle_tf2_listener
  geometry_msgs
  rclcpp
  tf2
  tf2_ros
  turtlesim
)

add_executable(frame_tf2_broadcaster
  src/frame_tf2_broadcaster.cpp
)
ament_target_dependencies(frame_tf2_broadcaster
  geometry_msgs
  rclcpp
  tf2
  tf2_ros
  turtlesim
)

install(TARGETS
  static_turtle_tf2_broadcaster
  turtle_tf2_broadcaster
  turtle_tf2_listener
  frame_tf2_broadcaster      # ← 新加的
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
