cmake_minimum_required(VERSION 3.8)
project(node_interfaces_tutorial)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(node_interfaces_example
  src/node_interfaces_example.cpp
)
target_link_libraries(node_interfaces_example
  rclcpp::rclcpp
)

# ===== 模板教程的可执行文件 =====
add_executable(node_interfaces_template_example
  src/node_interfaces_template_example.cpp
)
target_link_libraries(node_interfaces_template_example
  rclcpp::rclcpp
)

install(TARGETS
  node_interfaces_example
  node_interfaces_template_example
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
