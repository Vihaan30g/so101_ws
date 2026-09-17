import time
from pynput import keyboard

from lerobot.robots.so_follower.so_follower import (
    SOFollower,
    SOFollowerRobotConfig,
)

PORT = "/dev/ttyACM0"
ROBOT_ID = "vihu2"
STEP = 2.0  # degrees per key press

KEYMAP = {
    "q": ("shoulder_pan.pos", +STEP),
    "a": ("shoulder_pan.pos", -STEP),
    "w": ("shoulder_lift.pos", +STEP),
    "s": ("shoulder_lift.pos", -STEP),
    "e": ("elbow_flex.pos", +STEP),
    "d": ("elbow_flex.pos", -STEP),
    "r": ("wrist_flex.pos", +STEP),
    "f": ("wrist_flex.pos", -STEP),
    "t": ("gripper.pos", +STEP),
    "g": ("gripper.pos", -STEP),
}


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


def main():
    cfg = SOFollowerRobotConfig(port=PORT, id=ROBOT_ID)
    robot = SOFollower(cfg)

    print("Connecting to SO-101...")
    connect_with_retries(robot)
    print("Connected.")

    observation = robot.get_observation()
    pos = {k: v for k, v in observation.items() if k.endswith(".pos")}

    print("\nCurrent positions:")
    for joint, value in pos.items():
        print(f"  {joint}: {value}")

    def on_press(key):
        try:
            k = key.char
            if k in KEYMAP:
                joint, delta = KEYMAP[k]
                if joint not in pos:
                    print(f"Joint {joint} not found!")
                    return
                pos[joint] += delta
                robot.send_action(pos)
                print(f"{joint}: {pos[joint]:.2f}")
        except AttributeError:
            pass

    def on_release(key):
        if key == keyboard.Key.esc:
            print("\nStopping...")
            return False

    print("\n===================================")
    print(" SO-101 Keyboard Teleoperation")
    print("===================================")
    print("Q/A shoulder_pan   W/S shoulder_lift   E/D elbow_flex")
    print("R/F wrist_flex     T/G gripper         ESC stop")
    print(f"STEP = {STEP} degrees\n")

    try:
        with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
            listener.join()
    finally:
        print("Disconnecting robot...")
        try:
            robot.disconnect()
        except Exception as e:
            print(f"(disconnect raised: {e}, ignoring)")
        print("Robot disconnected.")


if __name__ == "__main__":
    main()