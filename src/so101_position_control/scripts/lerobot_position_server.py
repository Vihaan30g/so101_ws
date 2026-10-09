#!/usr/bin/env python3
"""LeRobot TCP server for direct SO-101 joint position commands.

Run this script from the same LeRobot Python environment used to calibrate
the robot. ROS commands and feedback use radians; LeRobot's SOFollower API
uses degrees for arm joints and a 0-100 open percentage for the gripper.
"""

import math
import os
import socket
import time

from lerobot.robots.so_follower.config_so_follower import SOFollowerRobotConfig
from lerobot.robots.so_follower.so_follower import SOFollower

JOINTS = (
    "shoulder_pan", "shoulder_lift", "elbow_flex",
    "wrist_flex", "wrist_roll", "gripper",
)
POSITION_LIMITS_RAD = {
    "shoulder_pan": (-1.91986, 1.91986),
    "shoulder_lift": (-1.74533, 1.74533),
    "elbow_flex": (-1.69, 1.69),
    "wrist_flex": (-1.65806, 1.65806),
    "wrist_roll": (-2.74385, 2.84121),
    "gripper": (-0.174533, 1.74533),
}
HOST = os.environ.get("SO101_HOST", "0.0.0.0")
PORT = int(os.environ.get("SO101_POSITION_PORT", "50011"))
ROBOT_PORT = os.environ.get("SO101_ROBOT_PORT", "/dev/ttyACM0")
ROBOT_ID = os.environ.get("SO101_ROBOT_ID", "vihu2")
STATE_PERIOD_S = float(os.environ.get("SO101_STATE_PERIOD_S", "0.02"))


def radians_to_lerobot_action(positions):
    action = {}
    for joint, value in zip(JOINTS, positions):
        if joint == "gripper":
            lower, upper = POSITION_LIMITS_RAD[joint]
            action["gripper.pos"] = (value - lower) * 100.0 / (upper - lower)
        else:
            action[f"{joint}.pos"] = math.degrees(value)
    return action


class PositionServer:
    def __init__(self):
        calibration = os.path.expanduser(os.environ.get(
            "SO101_CALIBRATION_FILE",
            f"~/.cache/huggingface/lerobot/calibration/robots/so_follower/{ROBOT_ID}.json",
        ))
        if not os.path.isfile(calibration):
            raise FileNotFoundError(f"LeRobot calibration file not found: {calibration}")

        config = SOFollowerRobotConfig(port=ROBOT_PORT, id=ROBOT_ID)
        self.robot = SOFollower(config)
        self.robot.connect(calibrate=False)
        missing = [joint for joint in JOINTS if joint not in self.robot.bus.calibration]
        if missing:
            self.robot.disconnect()
            raise RuntimeError("LeRobot calibration is missing: " + ", ".join(missing))

        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server.bind((HOST, PORT))
        self.server.listen(1)
        self.server.settimeout(0.25)

    def measured_positions(self):
        positions = []
        for joint in JOINTS:
            raw = self.robot.bus.read("Present_Position", joint, normalize=False)
            calibration = self.robot.bus.calibration[joint]
            raw_span = calibration.range_max - calibration.range_min
            if raw_span <= 0:
                raise ValueError(f"Invalid calibration range for {joint}")
            lower, upper = POSITION_LIMITS_RAD[joint]
            fraction = (raw - calibration.range_min) / raw_span
            positions.append(lower + fraction * (upper - lower))
        return positions

    def handle_client(self, connection):
        connection.settimeout(0.02)
        buffer = b""
        last_state = 0.0
        while True:
            try:
                data = connection.recv(4096)
            except socket.timeout:
                data = None
            if data == b"":
                return
            if data:
                buffer += data
                while b"\n" in buffer:
                    raw_line, buffer = buffer.split(b"\n", 1)
                    fields = raw_line.split()
                    if len(fields) != 8 or fields[0] != b"P1":
                        continue
                    try:
                        positions = [float(value) for value in fields[2:]]
                        if not all(math.isfinite(value) for value in positions):
                            continue
                        self.robot.send_action(radians_to_lerobot_action(positions))
                    except (ValueError, RuntimeError) as exc:
                        print(f"Position action failed: {exc}", flush=True)

            now = time.monotonic()
            if now - last_state >= STATE_PERIOD_S:
                try:
                    state = self.measured_positions()
                    line = " ".join(["S1", str(time.time()), *map(str, state), "0"]) + "\n"
                    connection.sendall(line.encode("ascii"))
                except (OSError, ValueError, RuntimeError) as exc:
                    print(f"State update failed: {exc}", flush=True)
                    if isinstance(exc, OSError):
                        return
                last_state = now

    def run(self):
        print(f"SO-101 position server listening on {HOST}:{PORT}", flush=True)
        try:
            while True:
                try:
                    connection, address = self.server.accept()
                except socket.timeout:
                    continue
                print(f"ROS bridge connected: {address}", flush=True)
                with connection:
                    self.handle_client(connection)
                print("ROS bridge disconnected", flush=True)
        finally:
            self.server.close()
            self.robot.disconnect()


if __name__ == "__main__":
    PositionServer().run()
