# so101_description

Provides the SO-101 robot description, meshes, and shared joint-limit
configuration.

## View the robot in RViz

Build and source the workspace, then run:

```bash
ros2 launch so101_description view_robot.launch.py
```

This opens RViz with the robot model and TF displays and starts the joint
state publisher GUI. Use the sliders to change the arm's joint positions.
