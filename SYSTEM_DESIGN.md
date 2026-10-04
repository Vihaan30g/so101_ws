# SO-101 Cartesian Servo Workspace

### **WORK IN PROGRESS**
There is a significant level of work left in this repo. I am building a CV-based master arm to replace expensive classic master arms for which one needs to spend again heavily on the motor actuators and 3d printing.

After that, we will be training the arm using both - reinforcement learning and imitation learning frameworks.

So stay tuned, i will add these projects very soon.

<br>
<br>

This workspace separates ROS 2 control logic from the LeRobot/Feetech hardware process. The hardware arm remains connected to the workstation; the future camera/IMU device only needs to publish the same ROS messages used by the keyboard test package.

## Architecture

```text
Cartesian device or keyboard
  -> /cartesian_cmd (so101_msgs/CartesianCommand)
  -> /wrist_cmd (so101_msgs/JointVelocityCommand)
  -> /gripper_cmd (std_msgs/Float64)
              |
              v
cartesian_controller_node
  -> /joint_cmd (six-joint velocity command)
  -> /commanded_joint_states (ghost visualization)
              |
              v
safety_gate_node
  -> /joint_cmd_safe
              |
              v
tcp_bridge_node
  -> TCP V1 lines on localhost
              |
              v
lerobot_server.py
  -> Feetech/LeRobot motors
  -> TCP S1 state lines
              |
              v
/actual_joint_states
```

The TCP protocol is deliberately unchanged from the working prototype:

```text
V1 timestamp v0 v1 v2 v3 v4 v5 control_mode
S1 timestamp p0 p1 p2 p3 p4 p5 status
```

The fixed order is `shoulder_pan`, `shoulder_lift`, `elbow_flex`, `wrist_flex`, `wrist_roll`, `gripper`. Velocities are rad/s and positions are radians. The bridge keeps the newest command under a mutex, while the LeRobot server protects command state and all bus I/O with separate locks.

## Cartesian behavior

Pinocchio loads the URDF configured by `so101_bringup/config/robot_params.yaml`. The default end-effector frame is `gripper_frame_link`, which is the frame at the wrist-roll/gripper end of the supplied URDF. Only `shoulder_pan`, `shoulder_lift`, and `elbow_flex` are included in the reduced translational Jacobian. Damped least-squares IK produces their velocities. `wrist_flex` and `wrist_roll` are direct orientation inputs; `gripper` is a direct velocity input.

The controller accepts both `velocity` and `pose` Cartesian commands. Pose mode uses proportional position control with a configured speed limit. Input messages expire and become zero commands when stale.

### Freeze and resume

`CartesianCommand.freeze_translation` is the device-neutral equivalent of the requested `b` control. On the rising edge, the controller records the measured end-effector position. While asserted, the first three joint velocities are zero, while wrist and gripper commands continue. On release in pose mode, the current device target is rebased so it equals the recorded end-effector position at the release instant. Subsequent device motion is then followed relative to that rebased target, avoiding a teleport.

The safety gate repeats the important part of this rule: any command marked `FROZEN_TRANSLATION` has its first three velocities forced to zero before it can reach the bridge.

## Visualization

The stack launch starts two `robot_state_publisher` nodes with the same URDF:

- commanded model: frames prefixed `commanded/`, joint input `/commanded_joint_states`, description `/commanded/robot_description`
- actual model: frames prefixed `actual/`, joint input `/actual_joint_states`, description `/actual/robot_description`

Static transforms connect both `commanded/base_link` and `actual/base_link` to `world`. In RViz2 set Fixed Frame to `world`, add two RobotModel displays, select the two description topics, and give the commanded model a lower Alpha such as `0.4`. The frame prefixes prevent TF collisions.

## Keyboard test package

`so101_input_keyboard` publishes the same topics expected from the future device. Run it in a terminal with a TTY:

- `i/k`, `j/l`, `u/o`: Cartesian X/Y/Z velocity bursts
- `a/d`: wrist flex velocity
- `z/x`: wrist roll velocity
- `[` / `]`: gripper velocity
- `b`: toggle translation freeze
- space: stop all motion
- `q`: quit

A terminal cannot report physical key-release events, so `b` is a toggle in this test node. The future ESP32/camera input node should publish `freeze_translation` true while its real button is held and false on release.

## Build and run

From `/home/marsuser/so101_ws`:

```bash
source /opt/ros/$ROS_DISTRO/setup.bash
colcon build --symlink-install --base-paths src
source install/setup.bash
ros2 launch so101_bringup so101_stack.launch.py
```

Start the LeRobot server separately in the Python environment that contains LeRobot:

```bash
python3 src/so101_bringup/scripts/lerobot_server.py
```

The server must be started before the ROS launch. On startup it prints the calibration file and all six calibration ranges. By default it uses `~/.cache/huggingface/lerobot/calibration/robots/<robot-id>.json`; set `SO101_CALIBRATION_FILE` if the SDK environment stores calibration elsewhere. The ROS bridge must then print `Connected to LeRobot TCP server` and `/actual_joint_states` must publish before the Cartesian controller will generate commands.

Then, in another sourced ROS terminal:

```bash
ros2 run so101_input_keyboard keyboard_servo
```

Useful checks in a sourced ROS terminal:

```bash
ros2 topic echo /actual_joint_states --once
ros2 topic echo /controller_status
ros2 topic echo /joint_cmd_safe
ros2 topic hz /actual_joint_states
```

Interpretation: no actual state means the LeRobot server or TCP connection is not working; an empty or zero safe command with `Rejected unsafe command` means the safety gate is rejecting the command; healthy state and safe commands with no motion points to the LeRobot server or motor bus. The old `/joint_states` topic is also published as a compatibility alias, but the control path uses `/actual_joint_states`.

The server's `SO101_PORT`, `SO101_ROBOT_PORT`, `SO101_ROBOT_ID`, timeout, and wrist-roll scale remain environment-configurable as in the prototype.

## Configuration and safety

Change joint grouping, end-effector frame, URDF path, rates, command timeouts, DLS damping, pose gain, speed limits, and joint-limit margin in `src/so101_bringup/config/robot_params.yaml`. Physical position and velocity limits are read from the URDF and checked by the safety gate; the LeRobot server also keeps position-mode joints in firmware-protected position mode and uses a watchdog.

The current kinematics package still reports self-collision as unavailable. Joint limits, velocity limits, stale-state checks, finite-value checks, and the LeRobot watchdog are active protections, but they do not replace a hardware emergency stop and supervised testing. Test at low speed with the arm clear of people and obstacles.
