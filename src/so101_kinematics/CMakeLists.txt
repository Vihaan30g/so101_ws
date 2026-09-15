cmake_minimum_required(VERSION 3.8)
project(so101_kinematics)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(Eigen3 REQUIRED)

# robotpkg-pinocchio installs under /opt/openrobots by default and is NOT
# found via the normal ROS rosdep/apt path. Make sure your shell has
# sourced /opt/openrobots/setup.sh (or equivalent) so that
# CMAKE_PREFIX_PATH / PKG_CONFIG_PATH include it BEFORE running colcon
# build. See top-level README.md, section "Pinocchio / robotpkg setup".
find_package(pinocchio REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/so101_kinematics.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  pinocchio::pinocchio
)

# pinocchio's exported CMake target normally brings in Boost / urdfdom /
# hpp-fcl transitively. If your specific robotpkg install does NOT export
# those transitively (version-dependent), uncomment and adjust:
# find_package(Boost REQUIRED COMPONENTS system filesystem)
# target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system Boost::filesystem)

add_executable(test_kinematics_node src/test_kinematics_node.cpp)
target_link_libraries(test_kinematics_node PRIVATE ${PROJECT_NAME})

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  TARGETS test_kinematics_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(Eigen3)
ament_export_include_directories(include)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # TODO(next session): add gtest-based unit tests here for:
  #   - FK against a couple of hand-verified poses of the real arm
  #   - Jacobian finite-difference check (numerically perturb q, compare
  #     to J * dq against the analytical FK delta)
  #   - DLS solve degrades gracefully as w -> 0 (no NaNs, bounded output)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
