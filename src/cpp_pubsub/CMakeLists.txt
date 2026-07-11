cmake_minimum_required(VERSION 3.8)
project(cpp_pubsub)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tutorial_interfaces REQUIRED)
add_executable(publisher src/publisher.cpp)
ament_target_dependencies(publisher rclcpp tutorial_interfaces)
add_executable(subscriber src/subscriber.cpp)
ament_target_dependencies(subscriber rclcpp tutorial_interfaces)

# 带 Topic Statistics 的 subscriber
add_executable(subscriber_with_stats src/subscriber_with_stats.cpp)
ament_target_dependencies(subscriber_with_stats rclcpp tutorial_interfaces)

install(TARGETS publisher subscriber subscriber_with_stats DESTINATION lib/${PROJECT_NAME})

# === Linter 自动检查 ===
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
