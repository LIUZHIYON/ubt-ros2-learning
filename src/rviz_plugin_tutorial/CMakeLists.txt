cmake_minimum_required(VERSION 3.8)
project(rviz_plugin_tutorial)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rviz_plugin_tutorial_msgs REQUIRED)
find_package(rviz_rendering REQUIRED)

# ===== Qt 的 MOC (Meta-Object Compiler) 必须 =====
set(CMAKE_AUTOMOC ON)

# 用 Qt5 的 MOC 处理头文件（里面有 Q_OBJECT 宏）
qt5_wrap_cpp(MOC_FILES
  include/rviz_plugin_tutorial/point_display.hpp
)

# ===== 构建共享库（插件必须是 .so）=====
add_library(${PROJECT_NAME} SHARED
  src/point_display.cpp
  ${MOC_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# ament_target_dependencies 会自动处理 include 路径和链接
ament_target_dependencies(${PROJECT_NAME}
  rviz_plugin_tutorial_msgs
)

target_link_libraries(${PROJECT_NAME}
  rclcpp::rclcpp
  rviz_common::rviz_common
  rviz_rendering::rviz_rendering
)

# ===== 注册插件（RViz 通过这个找到我们的插件）=====
pluginlib_export_plugin_description_file(rviz_common plugin_description.xml)

# ===== 安装 =====
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib/${PROJECT_NAME}
  LIBRARY DESTINATION lib/${PROJECT_NAME}
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY icons/
  DESTINATION share/${PROJECT_NAME}/icons
)

ament_export_dependencies(
  rclcpp
  rviz_common
  rviz_plugin_tutorial_msgs
  rviz_rendering
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
