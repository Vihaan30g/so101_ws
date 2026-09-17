cmake_minimum_required(VERSION 3.8)
project(so101_hardware_interface_cpp)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(diagnostic_msgs REQUIRED)
find_package(so101_msgs REQUIRED)

# Requires python3-dev installed system-wide: sudo apt install python3-dev
# This intentionally finds the SYSTEM Python (the one ROS 2 Humble was
# built against), not conda/venv -- we embed that interpreter and then
# manually add the venv's site-packages to sys.path at runtime instead,
# so the linked libpython stays ABI-matched with everything else in the
# workspace.
find_package(Python3 REQUIRED COMPONENTS Development)

add_executable(hardware_interface_node
  src/hardware_interface_node.cpp
  src/so_follower_bridge.cpp
)

target_include_directories(hardware_interface_node PRIVATE
  include
  ${Python3_INCLUDE_DIRS}
)

ament_target_dependencies(hardware_interface_node
  rclcpp
  sensor_msgs
  diagnostic_msgs
  so101_msgs
)

target_link_libraries(hardware_interface_node ${Python3_LIBRARIES})

install(TARGETS hardware_interface_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
