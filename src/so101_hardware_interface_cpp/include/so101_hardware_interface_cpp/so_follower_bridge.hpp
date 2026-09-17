#pragma once
// Wraps LeRobot's SOFollower via an embedded Python interpreter, using
// the raw CPython C-API (no pybind11 dependency needed for this small
// a surface area). All Python object handling is confined to this
// class -- hardware_interface_node.cpp never touches a PyObject*.
//
// Thread-safety: every public method acquires the GIL internally
// (PyGILState_Ensure/Release), so it's safe to call from any thread --
// important because rclcpp may run callbacks/timers on different
// threads depending on the executor/callback-group setup.

#include <map>
#include <string>
#include <stdexcept>

// Forward-declare to avoid forcing Python.h on every translation unit
// that includes this header.
struct _object;
typedef _object PyObject;

namespace so101_hardware_interface_cpp {

class SoFollowerBridge {
public:
  SoFollowerBridge();
  ~SoFollowerBridge();

  // Must be called once before anything else. venv_site_packages may be
  // empty to skip the sys.path insert (e.g. if lerobot is on the system
  // Python already). Throws std::runtime_error with a Python traceback
  // baked into the message on any failure.
  void initInterpreter(const std::string & venv_site_packages);

  // Connects to the robot (equivalent of robot.connect(calibrate=False)),
  // retrying `tries` times with `delay_s` seconds between attempts.
  void connect(const std::string & port, const std::string & robot_id,
               int tries = 3, double delay_s = 2.0);

  void disconnect() noexcept;  // never throws -- safe to call from a destructor path

  // Equivalent of robot.get_observation() -- returns raw keys exactly as
  // LeRobot gives them (e.g. "shoulder_pan.pos"), values in degrees.
  std::map<std::string, double> getObservation();

  // Equivalent of robot.send_action(action) -- keys like "shoulder_pan.pos",
  // values in degrees.
  void sendAction(const std::map<std::string, double> & action_deg);

private:
  PyObject * robot_obj_ = nullptr;   // strong ref to the SOFollower instance
  bool interpreter_owned_ = false;   // did WE call Py_Initialize (vs. already running)?
  bool connected_ = false;
};

}  // namespace so101_hardware_interface_cpp
