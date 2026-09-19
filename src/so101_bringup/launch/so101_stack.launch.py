from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterFile
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    pkg_share = get_package_share_directory('so101_bringup')
    params_file = os.path.join(pkg_share, 'config', 'robot_params.yaml')

    # FIX: robot_params.yaml uses "$(find-pkg-share so101_description)/..."
    # substitutions inside the YAML. Passing the raw path string to
    # `parameters=[...]` does NOT trigger substitution -- nodes would
    # receive the literal, unexpanded string as urdf_path and fail to
    # load it. Wrapping in ParameterFile(..., allow_substs=True) is what
    # actually expands it.
    params = ParameterFile(params_file, allow_substs=True)

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
    ])
