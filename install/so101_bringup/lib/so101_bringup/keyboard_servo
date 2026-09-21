#!/usr/bin/python3

import select
import sys
import termios
import time
import tty

import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node
from std_msgs.msg import Float64
from so101_msgs.msg import CartesianCommand


class KeyboardServo(Node):
    def __init__(self):
        super().__init__("keyboard_servo")
        self.cartesian_pub = self.create_publisher(CartesianCommand, "/cartesian_cmd", 10)
        self.gripper_pub = self.create_publisher(Float64, "/gripper_cmd", 10)
        self.speed = 0.03
        self.gripper_speed = 0.3
        self.last_velocity = (0.0, 0.0, 0.0)
        self.last_command_time = 0.0
        self.timer = self.create_timer(0.02, self.publish_command)

    def set_velocity(self, velocity):
        self.last_velocity = velocity
        self.last_command_time = time.monotonic()

    def stop(self):
        self.set_velocity((0.0, 0.0, 0.0))

    def publish_command(self):
        command = CartesianCommand()
        command.header.stamp = self.get_clock().now().to_msg()
        command.mode = "velocity"
        command.valid = (time.monotonic() - self.last_command_time) < 0.15
        command.velocity = Twist()
        command.velocity.linear.x, command.velocity.linear.y, command.velocity.linear.z = self.last_velocity
        self.cartesian_pub.publish(command)

    def publish_gripper(self, velocity):
        command = Float64()
        command.data = velocity
        self.gripper_pub.publish(command)


def main():
    rclpy.init()
    node = KeyboardServo()
    old_settings = termios.tcgetattr(sys.stdin)
    tty.setcbreak(sys.stdin.fileno())
    print("SO-101 keyboard servo: i/k x, j/l y, u/o z, [ ] gripper, space stop, q quit")
    print("Each key press commands a short Cartesian velocity burst.")
    try:
        while rclpy.ok():
            rclpy.spin_once(node, timeout_sec=0.01)
            ready, _, _ = select.select([sys.stdin], [], [], 0.01)
            if not ready:
                continue
            key = sys.stdin.read(1)
            if key == "q":
                break
            if key == "i":
                node.set_velocity((node.speed, 0.0, 0.0))
            elif key == "k":
                node.set_velocity((-node.speed, 0.0, 0.0))
            elif key == "j":
                node.set_velocity((0.0, node.speed, 0.0))
            elif key == "l":
                node.set_velocity((0.0, -node.speed, 0.0))
            elif key == "u":
                node.set_velocity((0.0, 0.0, node.speed))
            elif key == "o":
                node.set_velocity((0.0, 0.0, -node.speed))
            elif key == "[":
                node.publish_gripper(node.gripper_speed)
            elif key == "]":
                node.publish_gripper(-node.gripper_speed)
            elif key == " ":
                node.stop()
    finally:
        node.stop()
        for _ in range(3):
            rclpy.spin_once(node, timeout_sec=0.01)
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
