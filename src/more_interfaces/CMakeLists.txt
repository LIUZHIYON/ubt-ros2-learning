cmake_minimum_required(VERSION 3.8)
project(more_interfaces)

# =========================================
# 第 1 步：找到需要的包
# =========================================
find_package(ament_cmake REQUIRED)
# rosidl_default_generators：把 .msg 文件生成 C++ 头文件 / Python 模块
find_package(rosidl_default_generators REQUIRED)
# rclcpp：ROS2 C++ 客户端库（写节点用）
find_package(rclcpp REQUIRED)

# =========================================
# 第 2 步：生成接口（根据 .msg 生成代码）
# =========================================
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/AddressBook.msg"
  # 没有外部依赖（AddressBook 只用了 string 和 uint8，都是内置类型）
)

# =========================================
# 第 3 步：编译发布者 C++ 节点
# =========================================
add_executable(publish_address_book src/publish_address_book.cpp)

# 让编译器找到 ROS2 头文件 和 生成的接口头文件
target_include_directories(publish_address_book PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# 链接库：rclcpp（ROS 节点） + 本包的接口类型支持
target_link_libraries(publish_address_book
  rclcpp::rclcpp
  ${rosidl_TARGETS})

# ROS2 特有：链接类型支持
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} rosidl_typesupport_cpp)
target_link_libraries(publish_address_book ${cpp_typesupport_target})

# 安装可执行文件
install(TARGETS publish_address_book
  DESTINATION lib/${PROJECT_NAME})

# =========================================
# 第 4 步：导出依赖（让其他包能找到生成的接口）
# =========================================
ament_export_dependencies(rosidl_default_runtime)

# =========================================
# 第 5 步：CMake 收尾
# =========================================
ament_package()
