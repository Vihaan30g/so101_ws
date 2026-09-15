#!/usr/bin/env python3
"""Bridges the real SO-101 Feetech servo bus to ROS 2.

Wraps LeRobot's SOFollower driver rather than reimplementing a Feetech
bus driver. See package.xml for why this is Python instead of C++.

Contract:
  Subscribes:  /joint_cmd_safe  (so101_msgs/JointCommand)  velocities, rad/s
  Publishes:   /joint_states    (sensor_msgs/JointState)   positions, rad
               /diagnostics     (diagnostic_msgs/DiagnosticArray)

Design notes (see HANDOFF.md Step 2):
  - SOFollower.send_action() takes ABSOLUTE positions in degrees, not
    velocities. This node integrates commanded velocity into a running
    position setpoint per joint (same pattern as manual keyboard-step
    teleop, just with velocity*dt instead of a fixed step).
  - Degree<->radian conversion happens ONLY in this node.
  - Integration uses the REAL measured dt between ticks, not the nominal
    1/control_rate_hz, so a mismatch between configured and actually
    achievable rate cannot silently change commanded speed. Overrun
    ticks are reported via /diagnostics rather than assumed away.
  - On any per-tick exception, the node holds the last commanded
    position (does not integrate this tick, does not crash) per design
    doc §10.
"""

import math
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import JointState
from diagnostic_msgs.msg import DiagnosticArray, DiagnosticStatus, KeyValue

from so101_msgs.msg import JointCommand

try:
    from lerobot.robots.so_follower.so_follower import (
        SOFollower,
        SOFollowerRobotConfig,
    )
except ImportError as exc:  # pragma: no cover - environment-dependent
    SOFollower = None
    SOFollowerRobotConfig = None
    _IMPORT_ERROR = exc
else:
    _IMPORT_ERROR = None


def _connect_with_retries(robot, tries=3, delay=2.0, logger=None):
    for attempt in range(1, tries + 1):
        try:
            robot.connect(calibrate=False)
            return
        except ConnectionError as e:
            if logger:
                logger.warn(f"Connect attempt {attempt}/{tries} failed: {e}")
            if attempt == tries:
                raise
            time.sleep(delay)


class HardwareInterfaceNode(Node):

    def __init__(self):
        super().__init__('hardware_interface_node')

        if SOFollower is None:
            raise RuntimeError(
                'lerobot SOFollower could not be imported. Is lerobot '
                f'installed in this environment? Original error: {_IMPORT_ERROR}'
            )

        self.declare_parameter('port', '/dev/ttyACM0')
        self.declare_parameter('robot_id', 'vihu2')
        self.declare_parameter('control_rate_hz', 100.0)
        self.declare_parameter('joint_cmd_timeout_ms', 100)
        self.declare_parameter('tick_overrun_warn_ratio', 1.5)
        self.declare_parameter('joint_names', [
            'shoulder_pan', 'shoulder_lift', 'elbow_flex',
            'wrist_flex', 'wrist_roll', 'gripper',
        ])

        self._port = self.get_parameter('port').value
        self._robot_id = self.get_parameter('robot_id').value
        self._control_rate_hz = float(self.get_parameter('control_rate_hz').value)
        self._nominal_dt = 1.0 / self._control_rate_hz
        self._timeout_s = self.get_parameter('joint_cmd_timeout_ms').value / 1000.0
        self._overrun_ratio = self.get_parameter('tick_overrun_warn_ratio').value
        self._joint_names = list(self.get_parameter('joint_names').value)

        # --- connect to the real arm ---
        cfg = SOFollowerRobotConfig(port=self._port, id=self._robot_id)
        self._robot = SOFollower(cfg)
        self.get_logger().info(f'Connecting to SO-101 on {self._port} ...')
        _connect_with_retries(self._robot, logger=self.get_logger())
        self.get_logger().info('Connected.')

        # Seed the running position setpoint from the real arm so the
        # first tick doesn't jump the arm to some default position.
        obs = self._robot.get_observation()
        self._pos_deg = {
            j: obs[f'{j}.pos'] for j in self._joint_names if f'{j}.pos' in obs
        }
        missing = [j for j in self._joint_names if f'{j}.pos' not in obs]
        if missing:
            self.get_logger().error(
                f'Joints not found in SOFollower observation: {missing}. '
                'Check joint_names param against the real robot.'
            )

        self._latest_velocities = {j: 0.0 for j in self._joint_names}
        self._last_cmd_stamp = None  # rclpy Time of last received command
        self._last_tick_time = None  # time.monotonic() of previous tick

        # --- ROS I/O ---
        self._cmd_sub = self.create_subscription(
            JointCommand, '/joint_cmd_safe', self._on_joint_cmd,
            qos_profile_sensor_data,
        )
        self._state_pub = self.create_publisher(
            JointState, '/joint_states', qos_profile_sensor_data,
        )
        self._diag_pub = self.create_publisher(
            DiagnosticArray, '/diagnostics', 10,
        )

        self._timer = self.create_timer(self._nominal_dt, self._tick)

    # ------------------------------------------------------------------
    def _on_joint_cmd(self, msg: JointCommand):
        self._last_cmd_stamp = self.get_clock().now()
        for name, vel in zip(msg.joint_names, msg.velocities):
            if name in self._latest_velocities:
                self._latest_velocities[name] = vel
            else:
                self.get_logger().warn(
                    f'Received velocity for unknown joint "{name}", ignoring.'
                )

    # ------------------------------------------------------------------
    def _publish_diag(self, level, message, values=None):
        arr = DiagnosticArray()
        arr.header.stamp = self.get_clock().now().to_msg()
        status = DiagnosticStatus()
        status.level = level
        status.name = 'hardware_interface_node'
        status.message = message
        if values:
            status.values = [KeyValue(key=k, value=str(v)) for k, v in values.items()]
        arr.status.append(status)
        self._diag_pub.publish(arr)

    # ------------------------------------------------------------------
    def _tick(self):
        now_mono = time.monotonic()
        if self._last_tick_time is None:
            dt = self._nominal_dt
        else:
            dt = now_mono - self._last_tick_time
        self._last_tick_time = now_mono

        if dt > self._overrun_ratio * self._nominal_dt:
            self._publish_diag(
                DiagnosticStatus.WARN,
                'Tick overrun: bus/loop slower than configured control_rate_hz. '
                'Consider lowering control_rate_hz in hardware_params.yaml.',
                {'measured_dt_s': dt, 'nominal_dt_s': self._nominal_dt},
            )

        # Staleness: "user let go" / upstream stopped publishing -> zero velocity.
        stale = (
            self._last_cmd_stamp is None
            or (self.get_clock().now() - self._last_cmd_stamp).nanoseconds
            > self._timeout_s * 1e9
        )
        velocities = (
            {j: 0.0 for j in self._joint_names} if stale else self._latest_velocities
        )

        try:
            for j in self._joint_names:
                if j not in self._pos_deg:
                    continue
                self._pos_deg[j] += math.degrees(velocities.get(j, 0.0)) * dt

            self._robot.send_action(dict(self._pos_deg))
            obs = self._robot.get_observation()

            js = JointState()
            js.header.stamp = self.get_clock().now().to_msg()
            js.name = list(self._joint_names)
            js.position = [
                math.radians(obs.get(f'{j}.pos', float('nan')))
                for j in self._joint_names
            ]
            self._state_pub.publish(js)

        except Exception as e:  # noqa: BLE001 - deliberate broad catch, see §10
            self.get_logger().error(f'Hardware tick failed, holding position: {e}')
            self._publish_diag(
                DiagnosticStatus.ERROR,
                f'Hardware tick exception, holding last commanded position: {e}',
            )
            # Do not re-raise: skip this tick's motion, keep node alive.

    # ------------------------------------------------------------------
    def destroy_node(self):
        try:
            self._robot.disconnect()
        except Exception as e:  # noqa: BLE001
            self.get_logger().warn(f'disconnect() raised, ignoring: {e}')
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = HardwareInterfaceNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
