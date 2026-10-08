# SO-101 MoveIt Task Constructor demo

This package builds a MoveIt Task Constructor pick-and-place task for the
`body_grp` arm and `eef_grp` gripper in `so101_moveit_pkg`. It adds a cylinder
to MoveIt's planning scene, plans the gripper close/attach, arm transfer,
release/detach sequence, and executes the selected solution against the mock
`ros2_control` system while RViz displays the task and planning scene. The
pick pose is aligned with the robot's default zero-joint end-effector pose,
which keeps the initial grasp reachable with the SO-101's five-DOF arm.

## Build

Source ROS 2 Humble and the MoveIt/MTC overlay before building:

```bash
source /opt/ros/humble/setup.bash
source ~/ws_moveit2/install/setup.bash
cd ~/so101_ws
colcon build --symlink-install --base-paths src --packages-up-to so101_mtc
```

## Run

In a new terminal, source the same overlays in the same order and launch:

```bash
source /opt/ros/humble/setup.bash
source ~/ws_moveit2/install/setup.bash
source ~/so101_ws/install/setup.bash
ros2 launch so101_mtc pick_place.launch.py
```

The demo uses a fake hardware system and is for visualization/planning only; it
does not command the physical arm. The default pick pose is kinematically tied
to the default zero-joint state. If that start pose changes, update the
`grasp_frame` transform and cylinder coordinates in `src/pick_place_task.cpp`
and `config/task.yaml` together.
