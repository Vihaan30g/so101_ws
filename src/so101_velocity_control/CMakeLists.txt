cmake_minimum_required(VERSION 3.8)
project(so101_velocity_control)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(velocity_command_bridge src/velocity_command_bridge.cpp)
ament_target_dependencies(velocity_command_bridge rclcpp sensor_msgs)
target_include_directories(velocity_command_bridge PRIVATE include)

install(TARGETS velocity_command_bridge DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
install(PROGRAMS scripts/lerobot_velocity_server.py
  DESTINATION lib/${PROJECT_NAME})

ament_package()
