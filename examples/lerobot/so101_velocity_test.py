"""
Test raw joint VELOCITY control on the SO-101 follower, one joint at a time.

Bypasses robot.send_action() (POSITION-mode only) and talks directly to
FeetechMotorsBus to put a motor into VELOCITY (wheel) mode and give it a
raw Goal_Velocity command.

SAFETY: velocity/wheel mode does NOT respect position limits. Watch the
arm the whole time and be ready to Ctrl+C. Test wrist_roll first (it's
the one joint built for continuous rotation).
"""

import time

from lerobot.motors.feetech import OperatingMode
from lerobot.robots.so_follower.config_so_follower import SOFollowerRobotConfig
from lerobot.robots.so_follower.so_follower import SOFollower

PORT = "/dev/ttyACM0"
ROBOT_ID = "vihu2"

# Order matters: wrist_roll first (safe, continuous rotation joint).
# Comment out any joint you don't want to test yet.
JOINTS = ["wrist_roll", "wrist_flex", "elbow_flex", "shoulder_lift", "shoulder_pan"]

TEST_SPEED = 80        # raw Goal_Velocity units - START SMALL, no known deg/s mapping yet
TEST_DURATION_S = 1.5  # keep this short for finite-range joints


def set_mode(bus, motors, mode: OperatingMode):
    """Switching Operating_Mode needs torque briefly disabled (it's an EEPROM field)."""
    bus.disable_torque(motors)
    for m in motors:
        bus.write("Operating_Mode", m, mode.value)
    bus.enable_torque(motors)


def main():
    cfg = SOFollowerRobotConfig(port=PORT, id=ROBOT_ID)
    robot = SOFollower(cfg)

    print("Connecting...")
    robot.connect(calibrate=False)
    bus = robot.bus  # the FeetechMotorsBus - this is what we talk to directly

    try:
        for joint in JOINTS:
            input(f"\nPress ENTER to spin '{joint}' at speed={TEST_SPEED} for {TEST_DURATION_S}s "
                  f"(Ctrl+C anytime to abort safely) ...")

            set_mode(bus, [joint], OperatingMode.VELOCITY)

            start = time.time()
            bus.sync_write("Goal_Velocity", {joint: TEST_SPEED})
            while time.time() - start < TEST_DURATION_S:
                pos = bus.read("Present_Position", joint, normalize=False)
                vel = bus.read("Present_Velocity", joint, normalize=False)
                print(f"  {joint}: raw_pos={pos:5d}  raw_vel={vel:5d}")
                time.sleep(0.1)

            bus.sync_write("Goal_Velocity", {joint: 0})
            time.sleep(0.3)
            set_mode(bus, [joint], OperatingMode.POSITION)
            print(f"  '{joint}' stopped, back in POSITION mode.")

    finally:
        print("\nCleanup: forcing all motors to POSITION mode, zero velocity...")
        try:
            bus.sync_write("Goal_Velocity", dict.fromkeys(JOINTS, 0))
        except Exception as e:
            print(f"(couldn't zero velocity: {e})")
        try:
            set_mode(bus, JOINTS, OperatingMode.POSITION)
        except Exception as e:
            print(f"(couldn't reset mode: {e})")
        robot.disconnect()
        print("Disconnected.")


if __name__ == "__main__":
    main()
