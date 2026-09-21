# Full 6-joint output of controller / input of safety gate / input of hardware interface.
# Real joint names (not the earlier j1..j5 placeholder), kinematic-chain order:
#   ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
# gripper is direct-teleop passthrough, same as wrist_flex/wrist_roll -- not IK-controlled.
uint8 NORMAL = 0
uint8 FROZEN_TRANSLATION = 1

std_msgs/Header header
string[] joint_names     # ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
float64[] velocities
uint8 control_mode       # NORMAL or FROZEN_TRANSLATION
