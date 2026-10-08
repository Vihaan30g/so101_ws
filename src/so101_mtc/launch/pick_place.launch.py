from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler, TimerAction
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessExit
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    moveit_config = MoveItConfigsBuilder(
        "so101_new_calib", package_name="so101_moveit_pkg"
    ).to_moveit_configs()
    package_share = FindPackageShare("so101_mtc")
    controllers_file = PathJoinSubstitution(
        [package_share, "config", "ros2_controllers.yaml"]
    )
    rviz_file = PathJoinSubstitution(
        [package_share, "config", "so101_mtc.rviz"]
    )
    task_parameters = PathJoinSubstitution(
        [package_share, "config", "task.yaml"]
    )

    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            {
                "robot_description_kinematics": {
                    "body_grp": {"kinematics_solver_timeout": 0.1}
                }
            },
            {
                "capabilities": ParameterValue(
                    "move_group/ExecuteTaskSolutionCapability",
                    value_type=str,
                )
            },
        ],
    )
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="screen",
        parameters=[moveit_config.robot_description],
    )
    static_world_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        arguments=[
            "--x", "0",
            "--y", "0",
            "--z", "0",
            "--roll", "0",
            "--pitch", "0",
            "--yaw", "0",
            "--frame-id", "world",
            "--child-frame-id", "base_link",
        ],
        output="screen",
    )
    ros2_control = Node(
        package="controller_manager",
        executable="ros2_control_node",
        output="screen",
        parameters=[moveit_config.robot_description, controllers_file],
    )
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_file],
        condition=IfCondition(LaunchConfiguration("use_rviz")),
        output="screen",
        parameters=[moveit_config.to_dict()],
    )
    task_node = Node(
        package="so101_mtc",
        executable="so101_pick_place_task",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            {
                "robot_description_kinematics": {
                    "body_grp": {"kinematics_solver_timeout": 0.1}
                }
            },
            task_parameters,
        ],
    )

    controller_spawners = [
        Node(
            package="controller_manager",
            executable="spawner",
            arguments=[controller],
            output="screen",
        )
        for controller in (
            "joint_state_broadcaster",
            "body_grp_controller",
            "eef_grp_controller",
        )
    ]

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "use_rviz",
                default_value="true",
                description="Start RViz with the MTC task and planning-scene displays.",
            ),
            robot_state_publisher,
            static_world_tf,
            ros2_control,
            move_group,
            RegisterEventHandler(
                OnProcessExit(
                    target_action=controller_spawners[0],
                    on_exit=[controller_spawners[1]],
                )
            ),
            RegisterEventHandler(
                OnProcessExit(
                    target_action=controller_spawners[1],
                    on_exit=[controller_spawners[2]],
                )
            ),
            RegisterEventHandler(
                OnProcessExit(
                    target_action=controller_spawners[2],
                    on_exit=[TimerAction(period=2.0, actions=[task_node])],
                )
            ),
            controller_spawners[0],
            rviz,
        ]
    )
