from launch import LaunchDescription
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterFile, ParameterValue
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    pkg_share = get_package_share_directory('so101_bringup')
    description_share = get_package_share_directory('so101_description')
    params_file = os.path.join(pkg_share, 'config', 'robot_params.yaml')
    urdf_file = os.path.join(description_share, 'urdf', 'so101.urdf')

    # FIX: robot_params.yaml uses "$(find-pkg-share so101_description)/..."
    # substitutions inside the YAML. Passing the raw path string to
    # `parameters=[...]` does NOT trigger substitution -- nodes would
    # receive the literal, unexpanded string as urdf_path and fail to
    # load it. Wrapping in ParameterFile(..., allow_substs=True) is what
    # actually expands it.
    params = ParameterFile(params_file, allow_substs=True)

    robot_description = ParameterValue(Command(['cat ', urdf_file]), value_type=str)

    commanded_rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        namespace='commanded',
        name='commanded_state_publisher',
        parameters=[{'robot_description': robot_description, 'frame_prefix': 'commanded/'}],
        remappings=[('joint_states', '/commanded_joint_states')],
        output='screen',
    )
    actual_rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        namespace='actual',
        name='actual_state_publisher',
        parameters=[{'robot_description': robot_description, 'frame_prefix': 'actual/'}],
        remappings=[('joint_states', '/actual_joint_states')],
        output='screen',
    )

    return LaunchDescription([
        Node(
            package='so101_bringup',
            executable='cartesian_controller_node',
            name='cartesian_controller_node',
            parameters=[params],
            output='screen',
        ),
        Node(
            package='so101_bringup',
            executable='safety_gate_node',
            name='safety_gate_node',
            parameters=[params],
            output='screen',
        ),
        Node(
            package='so101_bringup',
            executable='tcp_bridge_node',
            name='tcp_bridge_node',
            parameters=[params],
            output='screen',
        ),
        commanded_rsp,
        actual_rsp,
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='world_to_commanded',
            arguments=['0', '0', '0', '0', '0', '0', 'world', 'commanded/base_link'],
            output='screen',
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='world_to_actual',
            arguments=['0', '0', '0', '0', '0', '0', 'world', 'actual/base_link'],
            output='screen',
        ),
    ])
