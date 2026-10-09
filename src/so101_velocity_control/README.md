# SO-101 direct velocity control

Subscribes to `sensor_msgs/msg/JointState` velocity commands and writes each
joint's velocity directly to its Feetech `Goal_Velocity` register through the
LeRobot motor-bus API. Commands are radians/second, ordered by `name`; all six
named joints are required. No velocity-to-position integration or current
position limit gate is used. Measured joint positions are published as
`sensor_msgs/msg/JointState` in radians.

## Build

From the workspace root (not `src`):

```bash
cd ~/Desktop/temp/so101_ws
source /opt/ros/humble/setup.bash
source ~/ws_moveit2/install/setup.bash
colcon build --packages-select so101_velocity_control
source install/setup.bash
```

The command topic defaults to `/so101/command/velocity`, feedback to
`/so101/actual_joint_states`, and TCP port to `50012`.

## Run

Start the server from the calibrated LeRobot Python environment:

```bash
python3 ~/Desktop/temp/so101_ws/src/so101_velocity_control/scripts/lerobot_velocity_server.py
```

Then run:

```bash
ros2 launch so101_velocity_control velocity_control.launch.py
```

Run only one SO-101 LeRobot server at a time; the position and velocity
servers both control the same serial-connected hardware.

Set `SO101_ROBOT_PORT`, `SO101_ROBOT_ID`, and optionally
`SO101_CALIBRATION_FILE` in the LeRobot environment. `Goal_Velocity` uses raw
servo units, so the server converts rad/s using
`SO101_VELOCITY_RAW_PER_RADSEC` (default `150.0`); calibrate this scale for the
actual servos. When the ROS bridge disconnects, the server writes zero velocity
to all joints. This package deliberately does not implement command-age
watchdogs; handle command safety at the higher level as planned.
