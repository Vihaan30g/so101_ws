# SO-101 Cartesian Servoing — Partial Implementation (Batch 1)

This is a **partial** implementation of the system described in
`so101_cartesian_control_design.md`, built in this order deliberately
because it's the order the design doc itself recommends (§11, build step 1):

> `so101_description` + `so101_kinematics`: load the real URDF, verify
> FK/Jacobian against known poses ... before any control logic exists.

## What's implemented in this batch

| Package | Status |
|---|---|
| `so101_msgs` | **Done.** All four messages from design doc §5, verbatim. |
| `so101_kinematics` | **Core done.** FK, full Jacobian, reduced 3xK translational Jacobian, manipulability index, adaptive-damping DLS IK solve, joint limit accessors. **Self-collision is a stub** (throws / returns false — see code comments and `HANDOFF.md`). |
| `so101_description` | **Skeleton only.** Ships a placeholder 5-joint URDF so the kinematics library has something to load and test against. **This is not the real SO-101** — see `so101_description/README.md` for what to swap in. |
| `so101_bringup` | **Skeleton only.** Holds `config/robot_params.yaml` (the shared param file the design doc specifies in §4). No launch files or RViz config yet. |
| `so101_input_keyboard`, `so101_cartesian_controller`, `so101_safety`, `so101_hardware_interface`, `so101_diagnostics` | **Not started.** See `HANDOFF.md`. |

Nothing here talks to real hardware yet. This batch corresponds to design
doc build-order step 1 only.

---

## 1. Prerequisites

- **Ubuntu 22.04 + ROS 2 Humble** (desktop or ros-base + `robot_state_publisher`, `xacro`, `rviz2` if you want visualization later).
- **robotpkg-pinocchio 4.1.0**, installed via robotpkg (not apt/rosdep). If you don't have robotpkg set up yet:

  ```bash
  # Add the robotpkg apt repo (see https://stack-of-tasks.github.io/pinocchio/download.html
  # for the exact current instructions/keys for your Ubuntu release), then:
  sudo apt update
  sudo apt install -y robotpkg-py3*-pinocchio   # or the non-python-bound variant if you don't need Python bindings
  ```

  robotpkg installs under `/opt/openrobots` by default. **You must have this
  on your CMake/pkg-config search path before building**, e.g. add to your
  `~/.bashrc`:

  ```bash
  export CMAKE_PREFIX_PATH=/opt/openrobots:$CMAKE_PREFIX_PATH
  export PKG_CONFIG_PATH=/opt/openrobots/lib/pkgconfig:$PKG_CONFIG_PATH
  export LD_LIBRARY_PATH=/opt/openrobots/lib:$LD_LIBRARY_PATH
  export PYTHONPATH=/opt/openrobots/lib/python3.10/site-packages:$PYTHONPATH  # only if you need pinocchio's python bindings
  ```

  Open a **new terminal** (or `source ~/.bashrc`) after adding these before
  building.

- Eigen3 (`sudo apt install libeigen3-dev` — usually already present with ROS Humble desktop).

## 2. Build

```bash
# from the workspace root (the directory containing src/)
source /opt/ros/humble/setup.bash

# confirm pinocchio is actually visible before wasting a build attempt:
pkg-config --modversion pinocchio   # should print something like 3.x / 4.x depending on robotpkg package version naming

colcon build --packages-select so101_msgs so101_kinematics so101_description so101_bringup
source install/setup.bash
```

If `find_package(pinocchio REQUIRED)` fails in `so101_kinematics`, it's
almost always the `CMAKE_PREFIX_PATH` step above not being sourced in the
same shell you're running `colcon build` from.

## 3. Sanity-check the kinematics library (build-order step 1)

```bash
ros2 run so101_kinematics test_kinematics_node \
  $(ros2 pkg prefix so101_description)/share/so101_description/urdf/so101.urdf
```

Expect output listing `nq`/`nv`, joint order, an EE position at `q=0`, the
reduced Jacobian, a manipulability number, and a DLS solve for a small
`+x` velocity command — all against the **placeholder** URDF. This just
proves the library builds, links against Pinocchio correctly, and the
math runs without throwing.

Once you've swapped in the real SO-101 description (see
`so101_description/README.md`), re-run this and compare the reported EE
position at a known joint configuration against the physical arm by hand,
per the design doc's build-order step 1.

## 4. What this batch deliberately does NOT do yet

- No ROS 2 *nodes* exist yet (the kinematics library is linked by a plain
  `main()`, not an `rclcpp::Node` — that's intentional, it's a pure sanity
  check, not part of the runtime graph).
- No hardware is touched.
- No safety gate, no controller, no keyboard input, no diagnostics.
- Self-collision checking in `so101_kinematics` is a stub.

See `HANDOFF.md` for the exact remaining scope, written so you can hand it
to a fresh Claude session (or pick it up yourself) and continue without
re-deriving the design.
