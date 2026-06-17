cmake_minimum_required(VERSION 3.8)
project(action_tutorials_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 生成 action 接口代码（action 会被自动拆成 msg + srv）
rosidl_generate_interfaces(${PROJECT_NAME}
  "action/Fibonacci.action"
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
