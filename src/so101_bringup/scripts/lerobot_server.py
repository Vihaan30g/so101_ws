#!/usr/bin/env python3
"""Standalone LeRobot TCP bridge for the SO-101.

This process intentionally avoids importing rclpy or any ROS Python package.
It runs in the user's working LeRobot Conda environment (Python 3.12) and
keeps robot I/O separate from ROS 2 networking (Python 3.10 / rclpy side,
see tcp_bridge_node.cpp).

SAFETY DESIGN (see review notes -- this replaces an earlier version that
drove ALL joints in raw VELOCITY/wheel mode):

  - shoulder_pan, shoulder_lift, elbow_flex, wrist_flex, gripper stay in
    POSITION mode (SOFollower's default after connect()). Commanded
    velocities for these are integrated in software into a target
    position, which is then sent via send_action(). This means the
    servo FIRMWARE itself rejects/clamps goal positions outside the
    calibrated travel range -- real protection that VELOCITY/wheel mode
    does not provide (wheel mode ignores position limits entirely).

  - wrist_roll is the one joint actually built for continuous rotation,
    so it alone is switched into VELOCITY mode and driven directly via
    Goal_Velocity.

UNITS ON THE WIRE (see tcp_protocol.hpp): commands are joint velocities
in rad/s, states are joint positions in RADIANS, for all 6 joints, in
fixed order. lerobot's own get_observation()/send_action() work in
DEGREES for 5 joints and a 0-100% "open-ness" for the gripper -- neither
of those is radians, so this file explicitly converts everything to/from
true physical radians (derived from raw encoder ticks + calibration) for
anything that crosses the TCP link. Reporting degrees or percent as if
they were radians (as the previous version did) would feed nonsense
positions into the C++ side's Pinocchio-based Jacobian/limit checks.
"""

import math
import os
import socket
import threading
import time
from typing import Dict, Optional

from lerobot.motors.feetech import OperatingMode
from lerobot.robots.so_follower.config_so_follower import SOFollowerRobotConfig
from lerobot.robots.so_follower.so_follower import SOFollower

JOINT_ORDER = [
    "shoulder_pan",
    "shoulder_lift",
    "elbow_flex",
    "wrist_flex",
    "wrist_roll",
    "gripper",
]

URDF_POSITION_LIMITS_RAD = {
    "shoulder_pan": (-1.91986, 1.91986),
    "shoulder_lift": (-1.74533, 1.74533),
    "elbow_flex": (-1.69, 1.69),
    "wrist_flex": (-1.65806, 1.65806),
    "wrist_roll": (-2.74385, 2.84121),
    "gripper": (-0.174533, 1.74533),
}

# Joints that stay in POSITION mode, driven by software-integrated velocity.
# The firmware enforces their calibrated travel limits -- this is the real
# safety net that VELOCITY/wheel mode does not provide.
POSITION_JOINTS = ["shoulder_pan", "shoulder_lift", "elbow_flex", "wrist_flex", "gripper"]

# The one joint actually built for continuous rotation.
VELOCITY_JOINTS = ["wrist_roll"]

FEETECH_TICKS_PER_REV = 4096  # STS3215: 12-bit encoder, 0..4095 raw ticks per revolution

PORT = int(os.environ.get("SO101_PORT", "50010"))
HOST = os.environ.get("SO101_HOST", "127.0.0.1")
ROBOT_PORT = os.environ.get("SO101_ROBOT_PORT", "/dev/ttyACM0")
ROBOT_ID = os.environ.get("SO101_ROBOT_ID", "vihu2")
COMMAND_TIMEOUT_S = float(os.environ.get("SO101_CMD_TIMEOUT_S", "0.25"))
STATE_PERIOD_S = float(os.environ.get("SO101_STATE_PERIOD_S", "0.02"))  # 50 Hz
CONTROL_PERIOD_S = float(os.environ.get("SO101_CONTROL_PERIOD_S", "0.01"))  # 100 Hz position integration


def connect_with_retries(robot, tries=3, delay=2.0):
    for attempt in range(1, tries + 1):
        try:
            robot.connect(calibrate=False)
            return
        except ConnectionError as e:
            print(f"Connect attempt {attempt}/{tries} failed: {e}")
            if attempt == tries:
                raise
            time.sleep(delay)


class So101LeRobotServer:
    def __init__(self):
        self.robot: Optional[SOFollower] = None
        self.bus = None

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((HOST, PORT))
        self.sock.listen(1)
        self.sock.settimeout(0.25)

        self.conn = None
        self.client_addr = None

        self.state_lock = threading.Lock()
        self.cmd_lock = threading.Lock()
        self.motor_io_lock = threading.Lock()

        # Commanded velocities, rad/s, indexed by JOINT_ORDER position. Written by
        # the socket-reading code, read by the control loop thread.
        self._cmd_velocities = [0.0] * len(JOINT_ORDER)
        self.last_command_ts = time.monotonic()

        # Software-integrated position targets for POSITION_JOINTS, in
        # lerobot's own native units (degrees for arm joints, 0-100% for
        # gripper) -- these are what get passed straight to send_action().
        self._pos_target = {}

        self.should_stop = False
        self.control_thread = None

    # ---------------------------------------------------------------- setup

    def _connect_robot(self):
        cfg = SOFollowerRobotConfig(port=ROBOT_PORT, id=ROBOT_ID)
        robot = SOFollower(cfg)
        with self.motor_io_lock:
            robot.connect(calibrate=False)  # connect() internally forces POSITION mode on ALL joints
            self.robot = robot
            self.bus = robot.bus

            # Seed integration targets from current measured position.
            obs = robot.get_observation()
            for j in POSITION_JOINTS:
                self._pos_target[j] = obs[f"{j}.pos"]

            # Only wrist_roll moves to VELOCITY mode. Everything else is left
            # exactly as connect() set it up (POSITION mode).
            self.bus.disable_torque(VELOCITY_JOINTS)
            for j in VELOCITY_JOINTS:
                self.bus.write("Operating_Mode", j, OperatingMode.VELOCITY.value)
            self.bus.enable_torque(VELOCITY_JOINTS)
            self.bus.sync_write("Goal_Velocity", {j: 0 for j in VELOCITY_JOINTS})

    def _shutdown_robot(self):
        with self.motor_io_lock:
            if self.bus is not None:
                try:
                    self.bus.sync_write("Goal_Velocity", {j: 0 for j in VELOCITY_JOINTS})
                except Exception:
                    pass
                try:
                    self.bus.disable_torque(VELOCITY_JOINTS)
                    for j in VELOCITY_JOINTS:
                        self.bus.write("Operating_Mode", j, OperatingMode.POSITION.value)
                    self.bus.enable_torque(VELOCITY_JOINTS)
                except Exception:
                    pass
            if self.robot is not None:
                try:
                    self.robot.disconnect()
                except Exception:
                    pass

    # --------------------------------------------------------- unit helpers

    def _raw_position(self, joint: str) -> int:
        with self.motor_io_lock:
            return self.bus.read("Present_Position", joint, normalize=False)

    def _raw_to_radians(self, joint: str, raw_tick: float) -> float:
        """Map calibrated encoder travel into the URDF joint coordinate."""
        cal = self.bus.calibration[joint]
        lower, upper = URDF_POSITION_LIMITS_RAD[joint]
        raw_span = cal.range_max - cal.range_min
        if raw_span <= 0:
            raise ValueError(f"invalid calibration range for {joint}")
        travel = (raw_tick - cal.range_min) / raw_span
        return lower + travel * (upper - lower)

    def _gripper_percent_per_radian(self) -> float:
        cal = self.bus.calibration["gripper"]
        raw_span = cal.range_max - cal.range_min
        if raw_span == 0:
            return 0.0
        rad_span = raw_span * (2.0 * math.pi / FEETECH_TICKS_PER_REV)
        return 100.0 / rad_span

    # --------------------------------------------------------- control loop

    def _control_tick(self, dt: float):
        with self.cmd_lock:
            velocities = list(self._cmd_velocities)
            stale = (time.monotonic() - self.last_command_ts) > COMMAND_TIMEOUT_S
        if stale:
            velocities = [0.0] * len(JOINT_ORDER)

        # --- POSITION_JOINTS: integrate rad/s into lerobot's native units ---
        action = {}
        pct_per_rad = None
        for i, joint in enumerate(JOINT_ORDER):
            if joint not in POSITION_JOINTS:
                continue
            v_rad_s = velocities[i]
            if joint == "gripper":
                if pct_per_rad is None:
                    pct_per_rad = self._gripper_percent_per_radian()
                delta = v_rad_s * pct_per_rad * dt
            else:
                delta = math.degrees(v_rad_s) * dt
            self._pos_target[joint] += delta
            action[f"{joint}.pos"] = self._pos_target[joint]

        try:
            with self.motor_io_lock:
                self.robot.send_action(action)
        except Exception as exc:
            print(f"send_action failed: {exc}")

        # --- VELOCITY_JOINTS: direct Goal_Velocity ---
        # NOTE: no fixed calibrated rad/s <-> raw-unit scale is assumed here;
        # tune SO101_WRIST_ROLL_RAW_PER_RADSEC to your servo if you need
        # accurate speed tracking. Direction/zero behavior is correct
        # regardless of this scale.
        raw_per_radsec = float(os.environ.get("SO101_WRIST_ROLL_RAW_PER_RADSEC", "150.0"))
        wrist_roll_idx = JOINT_ORDER.index("wrist_roll")
        raw_vel = int(round(velocities[wrist_roll_idx] * raw_per_radsec))
        try:
            with self.motor_io_lock:
                self.bus.sync_write("Goal_Velocity", {"wrist_roll": raw_vel})
        except Exception as exc:
            print(f"wrist_roll Goal_Velocity write failed: {exc}")

    def _control_loop(self):
        last = time.monotonic()
        while not self.should_stop:
            now = time.monotonic()
            dt = now - last
            last = now
            if self.robot is not None:
                self._control_tick(dt)
            time.sleep(CONTROL_PERIOD_S)

    # -------------------------------------------------------- wire protocol

    def _parse_line(self, line: str) -> Optional[Dict[str, object]]:
        line = line.strip()
        if not line:
            return None
        parts = line.split()
        if len(parts) < 9 or parts[0] != "V1":
            return None
        try:
            ts = float(parts[1])
            velocities = [float(v) for v in parts[2:8]]
            control_mode = int(parts[8])
        except ValueError:
            return None
        return {"ts": ts, "velocities": velocities, "control_mode": control_mode}

    def _apply_command(self, payload: Dict[str, object]):
        velocities = payload["velocities"]
        if not isinstance(velocities, list) or len(velocities) != 6:
            return
        with self.cmd_lock:
            self._cmd_velocities = list(velocities)
            self.last_command_ts = time.monotonic()

    def _read_state_radians(self):
        if self.robot is None:
            return None
        try:
            positions = []
            for joint in JOINT_ORDER:
                raw = self._raw_position(joint)
                positions.append(self._raw_to_radians(joint, raw))
            return positions
        except Exception as exc:
            print(f"State read failed: {exc}")
            return None

    def _send_state_line(self, positions_rad):
        if self.conn is None:
            return
        status = 0
        payload = ["S1", str(time.time()), *[str(float(p)) for p in positions_rad], str(status)]
        line = " ".join(payload) + "\n"
        try:
            self.conn.sendall(line.encode("utf-8"))
        except Exception:
            self.conn = None

    def _watchdog(self):
        with self.cmd_lock:
            timed_out = (time.monotonic() - self.last_command_ts) > COMMAND_TIMEOUT_S
        if timed_out:
            with self.cmd_lock:
                self._cmd_velocities = [0.0] * len(JOINT_ORDER)

    # ------------------------------------------------------------- serving

    def _serve(self):
        self._connect_robot()
        print("SO-101 LeRobot server connected.")

        self.control_thread = threading.Thread(target=self._control_loop, daemon=True)
        self.control_thread.start()

        last_state_ts = 0.0
        try:
            while not self.should_stop:
                if self.conn is None:
                    try:
                        self.conn, self.client_addr = self.sock.accept()
                        self.conn.settimeout(0.1)
                        print(f"Accepted connection from {self.client_addr}")
                    except socket.timeout:
                        self._watchdog()
                        continue
                    except OSError:
                        self._watchdog()
                        continue

                buffer = ""
                while not self.should_stop:
                    try:
                        data = self.conn.recv(4096)
                    except socket.timeout:
                        pass
                    except OSError:
                        self.conn = None
                        break
                    else:
                        if not data:
                            self.conn = None
                            break
                        buffer += data.decode("utf-8", errors="replace")
                        while "\n" in buffer:
                            line, buffer = buffer.split("\n", 1)
                            payload = self._parse_line(line)
                            if payload is not None:
                                self._apply_command(payload)

                    self._watchdog()

                    now = time.monotonic()
                    if now - last_state_ts >= STATE_PERIOD_S:
                        positions_rad = self._read_state_radians()
                        if positions_rad is not None:
                            self._send_state_line(positions_rad)
                        last_state_ts = now

        finally:
            self.should_stop = True
            if self.control_thread is not None:
                self.control_thread.join(timeout=2.0)
            self._shutdown_robot()
            try:
                self.sock.close()
            except Exception:
                pass
            print("LeRobot server stopped.")

    def run(self):
        self._serve()


if __name__ == "__main__":
    server = So101LeRobotServer()
    server.run()
