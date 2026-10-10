# SO-101 position control

Subscribes to a `sensor_msgs/msg/JointState` position command and sends the six
SO-101 joint targets to LeRobot over TCP. Input positions and measured feedback
are in radians, ordered by each message's `name` array. All six joints are
required; values are forwarded without checking measured position limits.


The bridge defaults to command topic `/so101/command/position`, feedback topic
`/so101/actual_joint_states`, and TCP port `50011`.



Run only one SO-101 LeRobot server at a time; the position and velocity
servers both control the same serial-connected hardware.





<br>
<br>

# How to run demo : 

### Terminal 1 

**cmd 1** <br>
```
__conda_setup="$('/home/marsuser/miniforge3/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/home/marsuser/miniforge3/etc/profile.d/conda.sh" ]; then
        . "/home/marsuser/miniforge3/etc/profile.d/conda.sh"
    else
        export PATH="/home/marsuser/miniforge3/bin:$PATH"
    fi
fi
unset __conda_setup
```

**cmd 2** <br>
```
conda activate lerobot
```

**cmd 3** <br>
```
python3 ./src/so101_position_control/scripts/lerobot_position_server.py
```


#### Note
You can change the topic name or remap it according to you use so on whichever topic you are getting the joint position values. check the launch file once it will be clear.


### Terminal 2 

**cmd 1** <br>
```
ros2 launch so101_position_control position_control.launch.py
```



<br>
<br>
<br>

Set `SO101_ROBOT_PORT`, `SO101_ROBOT_ID`, and optionally
`SO101_CALIBRATION_FILE` in the LeRobot environment as needed. The LeRobot API
expects degrees for the five arm joints and gripper open percentage; the server
converts the ROS radian targets into those API units.