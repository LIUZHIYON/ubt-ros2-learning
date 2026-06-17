cmake_minimum_required(VERSION 3.8)
project(composition_demo)

# 默认依赖
find_package(ament_cmake REQUIRED)

# ★ 组件必需
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)

# ===== Talker 组件 =====
add_library(talker_component SHARED
    src/talker_component.cpp
)
ament_target_dependencies(talker_component
    rclcpp rclcpp_components std_msgs
)
rclcpp_components_register_node(
    talker_component
    PLUGIN "composition_demo::Talker"
    EXECUTABLE talker
    EXECUTOR SingleThreadedExecutor
)

# ===== Listener 组件 =====
add_library(listener_component SHARED
    src/listener_component.cpp
)
ament_target_dependencies(listener_component
    rclcpp rclcpp_components std_msgs
)
rclcpp_components_register_node(
    listener_component
    PLUGIN "composition_demo::Listener"
    EXECUTABLE listener
    EXECUTOR SingleThreadedExecutor
)

# 安装所有组件
install(TARGETS
    talker_component
    listener_component
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装 launch 文件
install(DIRECTORY
    launch
    DESTINATION share/${PROJECT_NAME}
)

ament_package()
