cmake_minimum_required(VERSION 3.8)
project(so101_bringup)

find_package(ament_cmake REQUIRED)

# launch/ and rviz/ are currently empty placeholders -- see HANDOFF.md.
install(
  DIRECTORY launch rviz config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
