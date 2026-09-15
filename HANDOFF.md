# Handoff: SO-101 Cartesian Servoing — continue from Batch 1

## Progress update (this session, in-progress)

- Step 1 confirmed done: real `so101.urdf` (6 joints incl. `gripper`) is
  in `so101_description`, `robot_params.yaml` uses real joint names.
- User independently confirmed real hardware works via a standalone
  LeRobot script (`SOFollower`, port `/dev/ttyACM0`, robot id `vihu2`,
  motors calibrated). Confirmed joints: shoulder_pan, shoulder_lift,
  elbow_flex, wrist_flex, gripper (wrist_roll present in observation but
  NOT exercised by that script's keymap -- still needs a manual check).
- **New package `so101_hardware_interface`** (ament_python) added:
  `hardware_interface_node.py` wraps `SOFollower` directly (decision:
  Python rclpy node wrapping SOFollower, not a C++/pybind bridge, since
  SOFollower is Python-only and already working/calibrated). Subscribes
  `/joint_cmd_safe` (velocities, rad/s), integrates a per-joint position
  setpoint using REAL measured tick dt (not nominal 1/control_rate_hz),
  calls `send_action` (degrees), publishes `/joint_states` (radians) from
  `get_observation()`. Reports tick overruns and per-tick exceptions via
  `/diagnostics` instead of assuming a rate or crashing.
  Config: `so101_hardware_interface/config/hardware_params.yaml`
  (`control_rate_hz` nominal 100.0 -- NOT yet validated against real bus
  throughput; watch for WARN-level tick-overrun diagnostics when running
  it and lower if needed).
- Updated `so101_msgs/JointCommand.msg` doc comment: real 6-joint names,
  not the stale `j1..j5` placeholder (message wire format unaffected,
  string[]/float64[] already support 6 entries).
- **Still TODO from Step 2**: bringup launch file
  (`robot_state_publisher` + `hardware_interface_node` + `rviz2`); manual
  verification that `wrist_roll` moves correctly via `send_action`;
  running `hardware_interface_node` against the real arm to see actual
  tick-overrun behavior and tune `control_rate_hz` down if warned.


Paste this whole file as your first message to a new Claude conversation,
along with the `so101_ws/` workspace (as a zip or by uploading the files),
and the original `so101_cartesian_control_design.md`.

## Context

I'm implementing the system in `so101_cartesian_control_design.md` for a
real SO-101 arm, ROS 2 Humble, robotpkg-pinocchio 4.1.0. A previous Claude
session (limited by free-tier token budget) completed **Batch 1** only:

- `so101_msgs`: all 4 messages (`CartesianCommand`, `JointVelocityCommand`,
  `JointCommand`, `ControllerStatus`) — complete, matches design doc §5.
- `so101_kinematics`: C++ shared library wrapping Pinocchio —
  **forward kinematics, full Jacobian, reduced 3xK translational Jacobian,
  manipulability index, adaptive-damping DLS IK solve, joint limit
  accessors are DONE and should work.** `loadCollisionModel()` /
  `checkSelfCollision()` are **stubs** (throw / always return false) — see
  `so101_kinematics.hpp` doc comments and TODOs in
  `so101_kinematics.cpp` for exactly what's needed.
- `so101_description`: **placeholder** 5-joint URDF only, not the real
  robot. Real description needs to be swapped in from
  TheRobotStudio/SO-ARM100 or the user's existing URDF/xacro.
- `so101_bringup`: skeleton package holding `config/robot_params.yaml`
  (per design doc §4) — no launch files, no RViz config yet.
- `test_kinematics_node`: a plain (non-ROS) executable that sanity-checks
  the kinematics library against the placeholder URDF.

Nothing touches real hardware yet. No ROS 2 nodes (`rclcpp::Node`) exist
yet at all — everything so far is a library + a plain-main test harness.

## What's NOT done — please continue with these, roughly in this order

Follow the design doc's own build order (§11), continuing from step 1:

### Step 1 (finish it)
- [ ] Swap in the real SO-101 URDF/meshes into `so101_description` (get
  from TheRobotStudio/SO-ARM100 or the user's own file). Update
  `so101_bringup/config/robot_params.yaml` joint names / end-effector
  frame if they differ from the placeholder's `j1`..`j5` / `gripper_tip`.
- [ ] Run `test_kinematics_node` against the real URDF; ask the user to
  verify the reported EE position against the physical arm by hand at a
  known configuration.
- [ ] (Nice-to-have, not blocking) Add gtest unit tests for
  `so101_kinematics` per the TODO in its `CMakeLists.txt`.

### Step 2 — `so101_hardware_interface` + bare visualization
- [ ] Build (or adapt, per the design doc the user says they "likely
  already have most of this" — ASK before writing from scratch) a
  `hardware_interface_node` bridging the Feetech servo bus to ROS 2:
  subscribes `/joint_cmd_safe` (`so101_msgs/JointCommand`), publishes
  `/joint_states` (`sensor_msgs/JointState`) at the bus's real achievable
  rate (check this rate before assuming anything — design doc §9 flags
  this explicitly).
- [ ] `so101_bringup` launch file wiring `robot_state_publisher` +
  `hardware_interface_node` + `rviz2` so the user can confirm the **real**
  arm shows up correctly in RViz when moved by hand, before any control
  logic exists.

### Step 3 — `cartesian_controller_node`, simulation-only first
- [ ] `so101_input_keyboard`: keyboard teleop node publishing
  `CartesianCommand` (fixed-magnitude velocity per key while held, zero on
  release) and `JointVelocityCommand` for j4/j5, plus a `/freeze_button`
  `std_msgs/Bool` for the `b` hold-to-freeze behavior. Non-real-time,
  Python is fine per the design doc's stack note.
- [ ] `so101_cartesian_controller`: the real-time control loop node.
  Implement exactly the control law in design doc §6.1–§6.3 using the
  ALREADY-WORKING `so101_kinematics::So101Kinematics::solveDLS()` —
  don't reimplement the DLS math, call the library. Fixed-rate
  `rclcpp::create_wall_timer` per §9, NOT per-message reactive. Decouple
  input rate from control rate (keep last-received command, reuse each
  tick, see §9). Implement the two-mode freeze-button switch (§6.2)
  including the "command theta_dot=0 for one tick on the transition edge"
  detail — easy to forget, causes a visible jerk if skipped. Implement the
  `CartesianCommand` staleness timeout (§9, "user let go" → zero
  velocity). Publish `/commanded_joint_states` (pre-safety-gate, per §8)
  and `/controller_status`.
- [ ] Run **simulation-only** first (§11 step 3): don't wire to hardware
  yet, just confirm the ghost arm moves correctly in RViz for each key,
  including freeze-mode, before proceeding.

### Step 4 — `so101_safety`
- [ ] `safety_gate_node`: implement the full table from design doc §7 —
  joint position limits (clamp velocity to zero at boundary, don't clamp
  position), joint velocity limits (scale whole command vector
  proportionally), self-collision (rejects + holds last safe position —
  **this needs `so101_kinematics::loadCollisionModel()` /
  `checkSelfCollision()` implemented first**, see the TODO block in
  `so101_kinematics.cpp` for the exact Pinocchio calls needed:
  `pinocchio::urdf::buildGeom(..., pinocchio::COLLISION, ...)`,
  `addAllCollisionPairs()`, optional SRDF-based pair removal,
  `pinocchio::computeCollisions()`), singularity double-check (manipulability
  hard floor, zero translational command if breached even after DLS
  damping), and `/joint_cmd_raw` staleness timeout (§7 table, §9).
  Every rejection must populate `/diagnostics` with a specific
  human-readable reason string, not a generic one (§7, §10 — this is a
  named requirement, not a nice-to-have).
- [ ] Deliberately command something that SHOULD be rejected (joint limit,
  contrived self-collision config) and confirm the gate actually rejects
  it, before wiring to real hardware.

### Step 5 — connect to real hardware
- [ ] Only after 1–4 are solid per the design doc. Wire safety gate output
  to the real `hardware_interface_node`.

### Step 6 — `so101_diagnostics`
- [ ] Aggregator node subscribing `/diagnostics` from every node,
  republishing a flattened `/system_status`. Follow the ERROR-vs-WARN/INFO
  distinction in design doc §10 exactly (unreachable target ≠ bug;
  reserve ERROR for genuine faults). Do this last, once real failure modes
  from actual hardware use are known (design doc's explicit reasoning for
  ordering it last).

### Cross-cutting, do throughout (not a separate step)
- [ ] Every node: try/catch per-tick logic, degrade to safe hold state
  (zero velocity) on exception rather than letting the node die, per
  design doc §10.
- [ ] QoS per design doc §9: control-critical topics
  (`/joint_cmd_safe`, `/joint_states`) → `SensorDataQoS()` /
  `BEST_EFFORT` + small depth; diagnostics/status → default reliable.
- [ ] Use `CallbackGroup`s (MutuallyExclusive for the control timer,
  Reentrant for lower-priority subscriptions) in the controller node so a
  slow diagnostics publish can't stall the control tick (§9).
- [ ] Velocity ramp / simple low-pass filter on the raw keyboard-triggered
  velocity command before the IK solve (§9, "3–5 line first-order filter,
  not a trajectory planner" — keep it that simple).
- [ ] Write the RViz dual-arm config (`so101_bringup/rviz/dual_arm.rviz`)
  with two `RobotModel` displays on `real/` and `cmd/` TF prefixes, per
  §8, plus the launch file spawning two `robot_state_publisher` instances
  (namespaced `real`/`cmd`) as sketched in §8.

## Known gap: the gripper joint (discovered after Batch 1, not in original design doc)

The original `so101_cartesian_control_design.md` only ever specifies 5
joints (`j1`..`j5`). The REAL SO-101 URDF (`so101_new_calib.urdf`) has a
6th joint, `gripper` (jaw open/close), which the design doc's control
scheme does not cover at all. This is not an oversight to silently work
around -- implement it explicitly, as follows:

- Real joint names now in use (see `so101_bringup/config/robot_params.yaml`):
  `shoulder_pan`→j1, `shoulder_lift`→j2, `elbow_flex`→j3 (Cartesian/DLS-IK,
  `translational_joints`), `wrist_flex`→j4, `wrist_roll`→j5 (direct teleop,
  `wrist_joints`). `gripper` is separate from both lists.
- **Recommended approach (do this first, keep it simple):** treat
  `gripper` exactly like `wrist_flex`/`wrist_roll` already are -- direct
  velocity teleop, no IK. Concretely: add `"gripper"` as a third entry in
  the `joint_names` list of the existing `so101_msgs/JointVelocityCommand`
  published by `so101_input_keyboard` (no new message type needed), bind
  it to two keys (hold-to-close / hold-to-open, zero velocity on
  release -- same pattern as every other held-key input), and let
  `cartesian_controller_node` pass it straight through into the 6-joint
  `JointCommand` output alongside `wrist_flex`/`wrist_roll`, same as those
  two already do. `so101_safety`'s joint position/velocity limit checks
  (§7) apply to it the same as any other joint -- no special-casing
  needed there.
  - Note this means `JointCommand.joint_names` / `.velocities` in
    `so101_msgs` need to carry 6 entries, not 5, once `gripper` is added
    -- update the message's doc comment (currently says
    `["j1","j2","j3","j4","j5"]`) when you do this, don't leave it
    stale/misleading.
- **Optional later polish (not v1):** toggle-based open/close (one key
  press = ramp to fully open or fully closed, driven off servo position
  feedback) instead of hold-to-move. More pleasant to use, more logic
  (small state machine), not needed for a first working version -- do
  this only after the hold-to-move version is working reliably on the
  real arm.
- Do NOT block the rest of the implementation on deciding the gripper's
  final UX -- ship the simple hold-to-move version alongside the wrist
  joints in Step 3, and treat the toggle version as a separate later task.

## Constraints to respect

- ROS 2 **Humble**, C++ for control-critical nodes, Python allowed for
  non-real-time nodes (keyboard, diagnostics UI) — per the design doc's
  header.
- **robotpkg-pinocchio 4.1.0** — installed under `/opt/openrobots`, found
  via `CMAKE_PREFIX_PATH`, not a rosdep key. Don't add a `<depend>` on
  `pinocchio` to any `package.xml`.
- This needs to actually work on a **real physical SO-101 arm** — don't
  drift into a simulation-only deliverable. Flag clearly wherever you're
  making an assumption that needs verifying against the real hardware
  (servo bus achievable rate, real joint limits from the real URDF, real
  frame names, etc.).
- **If you're also running low on budget in the new session**: apply the
  same "build order, checkpoint, hand off" approach — finish whichever
  step you're on cleanly (don't leave a half-written node), update this
  handoff doc with what's newly done, and stop there rather than starting
  the next step thin.
