# SO-101 position control

Subscribes to a `sensor_msgs/msg/JointState` position command and sends the six
SO-101 joint targets to LeRobot over TCP. Input positions and measured feedback
are in radians, ordered by each message's `name` array. All six joints are
required; values are forwarded without checking measured position limits.

## Build

From the workspace root (not `src`):

```bash
cd ~/Desktop/temp/so101_ws
source /opt/ros/humble/setup.bash
source ~/ws_moveit2/install/setup.bash
colcon build --packages-select so101_position_control
source install/setup.bash
```

The bridge defaults to command topic `/so101/command/position`, feedback topic
`/so101/actual_joint_states`, and TCP port `50011`.

## Run

Start the LeRobot server from the calibrated LeRobot Python environment:

```bash
python3 ~/Desktop/temp/so101_ws/src/so101_position_control/scripts/lerobot_position_server.py
```

Then start ROS 2:

```bash
ros2 launch so101_position_control position_control.launch.py
```

Run only one SO-101 LeRobot server at a time; the position and velocity
servers both control the same serial-connected hardware.

Set `SO101_ROBOT_PORT`, `SO101_ROBOT_ID`, and optionally
`SO101_CALIBRATION_FILE` in the LeRobot environment as needed. The LeRobot API
expects degrees for the five arm joints and gripper open percentage; the server
converts the ROS radian targets into those API units.
