# Output of ANY input node (keyboard today, ArUco/IMU later), input of the controller.
std_msgs/Header header
string mode                    # "velocity" or "pose"   (start with "velocity" for keyboard)
geometry_msgs/Twist velocity   # linear part used; angular part ignored (translation-only control)
geometry_msgs/Pose target_pose # only used if mode == "pose"
bool valid                     # false if the input node itself detected bad/stale data
