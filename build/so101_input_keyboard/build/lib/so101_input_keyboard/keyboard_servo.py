#!/usr/bin/env python3

import select
import sys
import termios
import time
import tty

import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node
from std_msgs.msg import Float64
from so101_msgs.msg import CartesianCommand, JointVelocityCommand


class KeyboardServo(Node):
    def __init__(self):
        super().__init__('keyboard_servo')
        self.cartesian_pub = self.create_publisher(CartesianCommand, '/cartesian_cmd', 10)
        self.wrist_pub = self.create_publisher(JointVelocityCommand, '/wrist_cmd', 10)
        self.gripper_pub = self.create_publisher(Float64, '/gripper_cmd', 10)
        self.declare_parameter('cartesian_speed_mps', 0.03)
        self.declare_parameter('wrist_speed_rad_s', 0.4)
        self.declare_parameter('gripper_speed_rad_s', 0.3)
        self.declare_parameter('command_timeout_s', 0.15)
        self.speed = float(self.get_parameter('cartesian_speed_mps').value)
        self.wrist_speed = float(self.get_parameter('wrist_speed_rad_s').value)
        self.gripper_speed = float(self.get_parameter('gripper_speed_rad_s').value)
        self.timeout = float(self.get_parameter('command_timeout_s').value)
        self.last_velocity = (0.0, 0.0, 0.0)
        self.last_command_time = 0.0
        self.freeze_translation = False
        self.wrist_velocity = [0.0, 0.0]
        self.last_wrist_time = 0.0
        self.gripper_velocity = 0.0
        self.last_gripper_time = 0.0
        self.timer = self.create_timer(0.02, self.publish_command)

    def set_velocity(self, velocity):
        self.last_velocity = velocity
        self.last_command_time = time.monotonic()

    def stop(self):
        self.set_velocity((0.0, 0.0, 0.0))
        self.wrist_velocity = [0.0, 0.0]
        self.gripper_velocity = 0.0

    def publish_command(self):
        now = time.monotonic()
        stamp = self.get_clock().now().to_msg()
        cart = CartesianCommand()
        cart.header.stamp = stamp
        cart.mode = 'velocity'
        cart.valid = (now - self.last_command_time) < self.timeout
        cart.freeze_translation = self.freeze_translation
        cart.velocity = Twist()
        cart.velocity.linear.x, cart.velocity.linear.y, cart.velocity.linear.z = self.last_velocity
        self.cartesian_pub.publish(cart)

        wrist = JointVelocityCommand()
        wrist.header.stamp = stamp
        wrist.joint_names = ['wrist_flex', 'wrist_roll']
        wrist.velocities = self.wrist_velocity if (now - self.last_wrist_time) < self.timeout else [0.0, 0.0]
        self.wrist_pub.publish(wrist)

        gripper = Float64()
        gripper.data = self.gripper_velocity if (now - self.last_gripper_time) < self.timeout else 0.0
        self.gripper_pub.publish(gripper)


def main(args=None):
    rclpy.init(args=args)
    node = KeyboardServo()
    old_settings = termios.tcgetattr(sys.stdin)
    tty.setcbreak(sys.stdin.fileno())
    print('SO-101 keyboard input: i/k x, j/l y, u/o z, a/d wrist flex, z/x wrist roll, [/] gripper')
    print('space stops motion; b toggles translation freeze; q quits')
    try:
        while rclpy.ok():
            rclpy.spin_once(node, timeout_sec=0.01)
            ready, _, _ = select.select([sys.stdin], [], [], 0.01)
            if not ready:
                continue
            key = sys.stdin.read(1)
            if key == 'q':
                break
            if key == 'i':
                node.set_velocity((node.speed, 0.0, 0.0))
            elif key == 'k':
                node.set_velocity((-node.speed, 0.0, 0.0))
            elif key == 'j':
                node.set_velocity((0.0, node.speed, 0.0))
            elif key == 'l':
                node.set_velocity((0.0, -node.speed, 0.0))
            elif key == 'u':
                node.set_velocity((0.0, 0.0, node.speed))
            elif key == 'o':
                node.set_velocity((0.0, 0.0, -node.speed))
            elif key == 'a':
                node.wrist_velocity[0] = node.wrist_speed
                node.last_wrist_time = time.monotonic()
            elif key == 'd':
                node.wrist_velocity[0] = -node.wrist_speed
                node.last_wrist_time = time.monotonic()
            elif key == 'z':
                node.wrist_velocity[1] = node.wrist_speed
                node.last_wrist_time = time.monotonic()
            elif key == 'x':
                node.wrist_velocity[1] = -node.wrist_speed
                node.last_wrist_time = time.monotonic()
            elif key == '[':
                node.gripper_velocity = node.gripper_speed
                node.last_gripper_time = time.monotonic()
            elif key == ']':
                node.gripper_velocity = -node.gripper_speed
                node.last_gripper_time = time.monotonic()
            elif key == 'b':
                node.freeze_translation = not node.freeze_translation
                node.get_logger().info('Translation freeze: %s', 'ON' if node.freeze_translation else 'OFF')
            elif key == ' ':
                node.stop()
    finally:
        node.stop()
        for _ in range(3):
            rclpy.spin_once(node, timeout_sec=0.01)
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
