cmake_minimum_required(VERSION 3.8)
project(so101_bringup)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(so101_msgs REQUIRED)
find_package(so101_kinematics REQUIRED)
find_package(pinocchio REQUIRED) # <--- ADD THIS LINE

# Executables
add_executable(cartesian_controller_node src/cartesian_controller_node.cpp)
add_executable(safety_gate_node src/safety_gate_node.cpp)
add_executable(tcp_bridge_node src/tcp_bridge_node.cpp)

# Include Directories
target_include_directories(tcp_bridge_node PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/so101_bringup
)

# Target Dependencies & Linking
ament_target_dependencies(cartesian_controller_node rclcpp sensor_msgs std_msgs so101_msgs)
target_link_libraries(cartesian_controller_node so101_kinematics::so101_kinematics)

ament_target_dependencies(safety_gate_node rclcpp sensor_msgs so101_msgs)
target_link_libraries(safety_gate_node so101_kinematics::so101_kinematics)

ament_target_dependencies(tcp_bridge_node rclcpp sensor_msgs so101_msgs)

# Installation
install(
  DIRECTORY launch config #rviz
  DESTINATION share/${PROJECT_NAME}
)

install(
  PROGRAMS scripts/keyboard_servo.py
  DESTINATION lib/${PROJECT_NAME}
  RENAME keyboard_servo
)

install(TARGETS
  cartesian_controller_node
  safety_gate_node
  tcp_bridge_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()