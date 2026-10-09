#!/usr/bin/env python3
"""LeRobot TCP server for direct SO-101 joint velocity commands.

Velocity commands are rad/s and are written to each servo's Goal_Velocity
register in VELOCITY mode. No velocity is integrated into position and no
position feedback/range check gates a command. Tune the raw-unit conversion
for the particular servo/bus; the stock default is 150 raw units per rad/s.
"""

import math
import os
import socket
import time

from lerobot.motors.feetech import OperatingMode
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
PORT = int(os.environ.get("SO101_VELOCITY_PORT", "50012"))
ROBOT_PORT = os.environ.get("SO101_ROBOT_PORT", "/dev/ttyACM0")
ROBOT_ID = os.environ.get("SO101_ROBOT_ID", "vihu2")
STATE_PERIOD_S = float(os.environ.get("SO101_STATE_PERIOD_S", "0.02"))
RAW_PER_RAD_S = float(os.environ.get("SO101_VELOCITY_RAW_PER_RADSEC", "150.0"))


class VelocityServer:
    def __init__(self):
        if not math.isfinite(RAW_PER_RAD_S) or RAW_PER_RAD_S <= 0:
            raise ValueError("SO101_VELOCITY_RAW_PER_RADSEC must be finite and positive")
        calibration = os.path.expanduser(os.environ.get(
            "SO101_CALIBRATION_FILE",
            f"~/.cache/huggingface/lerobot/calibration/robots/so_follower/{ROBOT_ID}.json",
        ))
        if not os.path.isfile(calibration):
            raise FileNotFoundError(f"LeRobot calibration file not found: {calibration}")

        self.robot = SOFollower(SOFollowerRobotConfig(port=ROBOT_PORT, id=ROBOT_ID))
        self.robot.connect(calibrate=False)
        missing = [joint for joint in JOINTS if joint not in self.robot.bus.calibration]
        if missing:
            self.robot.disconnect()
            raise RuntimeError("LeRobot calibration is missing: " + ", ".join(missing))

        self.bus = self.robot.bus
        try:
            self.bus.disable_torque(list(JOINTS))
            for joint in JOINTS:
                self.bus.write("Operating_Mode", joint, OperatingMode.VELOCITY.value)
            self.bus.enable_torque(list(JOINTS))
            self.write_velocities([0.0] * len(JOINTS))
        except Exception:
            self.robot.disconnect()
            raise

        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server.bind((HOST, PORT))
        self.server.listen(1)
        self.server.settimeout(0.25)

    def write_velocities(self, velocities_rad_s):
        raw = {
            joint: int(round(velocity * RAW_PER_RAD_S))
            for joint, velocity in zip(JOINTS, velocities_rad_s)
        }
        self.bus.sync_write("Goal_Velocity", raw)

    def measured_positions(self):
        positions = []
        for joint in JOINTS:
            raw = self.bus.read("Present_Position", joint, normalize=False)
            calibration = self.bus.calibration[joint]
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
                    if len(fields) != 8 or fields[0] != b"W1":
                        continue
                    try:
                        velocities = [float(value) for value in fields[2:]]
                        if not all(math.isfinite(value) for value in velocities):
                            continue
                        self.write_velocities(velocities)
                    except (ValueError, RuntimeError) as exc:
                        print(f"Goal_Velocity write failed: {exc}", flush=True)

            now = time.monotonic()
            if now - last_state >= STATE_PERIOD_S:
                try:
                    positions = self.measured_positions()
                    line = " ".join(["S1", str(time.time()), *map(str, positions), "0"]) + "\n"
                    connection.sendall(line.encode("ascii"))
                except (OSError, ValueError, RuntimeError) as exc:
                    print(f"State update failed: {exc}", flush=True)
                    if isinstance(exc, OSError):
                        return
                last_state = now

    def run(self):
        print(
            f"SO-101 direct velocity server listening on {HOST}:{PORT}; "
            f"scale={RAW_PER_RAD_S} raw units/(rad/s)",
            flush=True,
        )
        try:
            while True:
                try:
                    connection, address = self.server.accept()
                except socket.timeout:
                    continue
                print(f"ROS bridge connected: {address}", flush=True)
                with connection:
                    self.handle_client(connection)
                try:
                    self.write_velocities([0.0] * len(JOINTS))
                except Exception as exc:
                    print(f"Failed to stop motors after bridge disconnect: {exc}", flush=True)
                print("ROS bridge disconnected; zero velocity sent", flush=True)
        finally:
            try:
                self.write_velocities([0.0] * len(JOINTS))
                self.bus.disable_torque(list(JOINTS))
            finally:
                self.server.close()
                self.robot.disconnect()


if __name__ == "__main__":
    VelocityServer().run()
