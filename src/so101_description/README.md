# so101_description

Ships a **placeholder** `urdf/so101.urdf` right now -- 5 revolute joints
(`j1`..`j5`) in series with made-up link lengths/inertias and a frame named
`gripper_tip`, just enough structure for `so101_kinematics` to load and for
`test_kinematics_node` to run against something.

## What to replace it with

1. Get the real description from
   [TheRobotStudio/SO-ARM100](https://github.com/TheRobotStudio/SO-ARM100)
   (or whatever URDF/xacro you already use to drive the physical arm), and
   its meshes.
2. Drop the URDF/xacro into `urdf/` and meshes into `meshes/`.
3. Keep the joint names `j1`..`j5` if you can (least change elsewhere), or
   if the real file uses different names, update
   `translational_joints` / `wrist_joints` in `config/robot_params.yaml`
   (in `so101_bringup`) to match.
4. Make sure there's a frame (link or explicit `<link>`) you can name as
   the end-effector -- update `end_effector_frame` in
   `config/robot_params.yaml` if it isn't called `gripper_tip`.
5. Re-run `test_kinematics_node` (see top-level README) against the real
   file and sanity-check the reported EE position at a known pose by hand
   against the physical arm, per build-order step 1 in the design doc.
6. If you use xacro, add a small CMake step to expand it to URDF at build
   time (`xacro` is already in `package.xml` exec_depend) -- not done yet
   here since the placeholder is plain URDF.
