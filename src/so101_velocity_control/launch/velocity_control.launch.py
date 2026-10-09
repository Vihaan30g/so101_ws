from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('host', default_value='127.0.0.1'),
        DeclareLaunchArgument('port', default_value='50012'),
        DeclareLaunchArgument('command_topic', default_value='/so101/command/velocity'),
        DeclareLaunchArgument('state_topic', default_value='/so101/actual_joint_states'),
        Node(
            package='so101_velocity_control',
            executable='velocity_command_bridge',
            name='so101_velocity_command_bridge',
            parameters=[{
                'host': LaunchConfiguration('host'),
                'port': LaunchConfiguration('port'),
                'command_topic': LaunchConfiguration('command_topic'),
                'state_topic': LaunchConfiguration('state_topic'),
            }],
            output='screen',
        ),
    ])
