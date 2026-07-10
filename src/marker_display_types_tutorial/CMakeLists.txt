cmake_minimum_required(VERSION 3.8)
project(marker_display_types_tutorial)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)

add_executable(all_markers_demo src/all_markers_demo.cpp)
target_include_directories(all_markers_demo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(all_markers_demo PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(
  all_markers_demo
  "geometry_msgs"
  "rclcpp"
  "visualization_msgs"
)

install(TARGETS all_markers_demo
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
