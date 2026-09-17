#include "so101_hardware_interface_cpp/so_follower_bridge.hpp"

#include <Python.h>
#include <thread>
#include <chrono>

namespace so101_hardware_interface_cpp
{

namespace
{

// Pulls the current Python exception (if any) into a readable string and
// clears it. Caller must hold the GIL.
std::string fetchPyError()
{
  if (!PyErr_Occurred()) {
    return "(no Python exception set)";
  }
  PyObject * type = nullptr;
  PyObject * value = nullptr;
  PyObject * traceback = nullptr;
  PyErr_Fetch(&type, &value, &traceback);
  PyErr_NormalizeException(&type, &value, &traceback);

  std::string msg = "(unprintable Python exception)";
  if (value) {
    PyObject * str_obj = PyObject_Str(value);
    if (str_obj) {
      const char * str = PyUnicode_AsUTF8(str_obj);
      if (str) {
        msg = str;
      }
      Py_DECREF(str_obj);
    }
  }
  Py_XDECREF(type);
  Py_XDECREF(value);
  Py_XDECREF(traceback);
  return msg;
}

}  // namespace

SoFollowerBridge::SoFollowerBridge() {}

SoFollowerBridge::~SoFollowerBridge()
{
  disconnect();
  if (interpreter_owned_ && Py_IsInitialized()) {
    PyGILState_STATE gstate = PyGILState_Ensure();
    Py_XDECREF(robot_obj_);
    robot_obj_ = nullptr;
    // Intentionally NOT calling Py_Finalize(): finalizing an embedded
    // interpreter that has loaded heavy extension modules (torch, etc.)
    // is a well-known source of shutdown crashes. Letting process exit
    // reclaim everything is simpler and safe here since this only
    // happens once, at node shutdown.
    PyGILState_Release(gstate);
  }
}

void SoFollowerBridge::initInterpreter(const std::string & venv_site_packages)
{
  if (!Py_IsInitialized()) {
    Py_Initialize();
    interpreter_owned_ = true;
  }
  PyGILState_STATE gstate = PyGILState_Ensure();

  if (!venv_site_packages.empty()) {
    // Insert via the C API directly (list insert), not by formatting the
    // path into a PyRun_SimpleString source string -- avoids quoting/
    // escaping bugs if the path ever contains a quote character.
    PyObject * sys_module = PyImport_ImportModule("sys");
    PyObject * sys_path = PyObject_GetAttrString(sys_module, "path");
    PyObject * path_str = PyUnicode_FromString(venv_site_packages.c_str());
    PyList_Insert(sys_path, 0, path_str);
    Py_DECREF(path_str);
    Py_DECREF(sys_path);
    Py_DECREF(sys_module);
  }

  PyGILState_Release(gstate);
}

void SoFollowerBridge::connect(
  const std::string & port, const std::string & robot_id, int tries, double delay_s)
{
  PyGILState_STATE gstate = PyGILState_Ensure();

  PyObject * module = PyImport_ImportModule("lerobot.robots.so_follower.so_follower");
  if (!module) {
    std::string err = fetchPyError();
    PyGILState_Release(gstate);
    throw std::runtime_error(
            "Failed to import lerobot.robots.so_follower.so_follower: " + err +
            " -- check venv_site_packages param points at an env with lerobot installed.");
  }

  PyObject * cfg_class = PyObject_GetAttrString(module, "SOFollowerRobotConfig");
  PyObject * follower_class = PyObject_GetAttrString(module, "SOFollower");
  Py_DECREF(module);
  if (!cfg_class || !follower_class) {
    std::string err = fetchPyError();
    Py_XDECREF(cfg_class);
    Py_XDECREF(follower_class);
    PyGILState_Release(gstate);
    throw std::runtime_error("Failed to find SOFollower/SOFollowerRobotConfig: " + err);
  }

  PyObject * cfg_kwargs = PyDict_New();
  PyDict_SetItemString(cfg_kwargs, "port", PyUnicode_FromString(port.c_str()));
  PyDict_SetItemString(cfg_kwargs, "id", PyUnicode_FromString(robot_id.c_str()));
  PyObject * empty_args = PyTuple_New(0);
  PyObject * cfg_obj = PyObject_Call(cfg_class, empty_args, cfg_kwargs);
  Py_DECREF(cfg_class);
  Py_DECREF(cfg_kwargs);
  if (!cfg_obj) {
    std::string err = fetchPyError();
    Py_DECREF(empty_args);
    Py_DECREF(follower_class);
    PyGILState_Release(gstate);
    throw std::runtime_error("Failed to construct SOFollowerRobotConfig: " + err);
  }

  PyObject * follower_args = PyTuple_Pack(1, cfg_obj);
  PyObject * robot = PyObject_CallObject(follower_class, follower_args);
  Py_DECREF(follower_class);
  Py_DECREF(follower_args);
  Py_DECREF(cfg_obj);
  Py_DECREF(empty_args);
  if (!robot) {
    std::string err = fetchPyError();
    PyGILState_Release(gstate);
    throw std::runtime_error("Failed to construct SOFollower: " + err);
  }

  robot_obj_ = robot;  // keep a strong reference for the node's lifetime

  bool ok = false;
  std::string last_err;
  for (int attempt = 1; attempt <= tries; ++attempt) {
    PyObject * kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "calibrate", Py_False);
    PyObject * args = PyTuple_New(0);
    PyObject * method = PyObject_GetAttrString(robot_obj_, "connect");
    PyObject * result = PyObject_Call(method, args, kwargs);
    Py_DECREF(method);
    Py_DECREF(args);
    Py_DECREF(kwargs);
    if (result) {
      Py_DECREF(result);
      ok = true;
      break;
    }
    last_err = fetchPyError();
    if (attempt < tries) {
      PyGILState_Release(gstate);
      std::this_thread::sleep_for(std::chrono::duration<double>(delay_s));
      gstate = PyGILState_Ensure();
    }
  }

  PyGILState_Release(gstate);

  if (!ok) {
    throw std::runtime_error("robot.connect() failed after " + std::to_string(tries) +
            " attempts: " + last_err);
  }
  connected_ = true;
}

void SoFollowerBridge::disconnect() noexcept
{
  if (!connected_ || !robot_obj_ || !Py_IsInitialized()) {
    return;
  }
  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject * method = PyObject_GetAttrString(robot_obj_, "disconnect");
  if (method) {
    PyObject * result = PyObject_CallObject(method, nullptr);
    Py_XDECREF(result);
    Py_DECREF(method);
    if (PyErr_Occurred()) {
      PyErr_Clear();  // best-effort on shutdown; nothing sensible to do with it
    }
  }
  PyGILState_Release(gstate);
  connected_ = false;
}

std::map<std::string, double> SoFollowerBridge::getObservation()
{
  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject * method = PyObject_GetAttrString(robot_obj_, "get_observation");
  PyObject * result = method ? PyObject_CallObject(method, nullptr) : nullptr;
  Py_XDECREF(method);
  if (!result) {
    std::string err = fetchPyError();
    PyGILState_Release(gstate);
    throw std::runtime_error("get_observation() failed: " + err);
  }

  std::map<std::string, double> obs;
  PyObject * key = nullptr;
  PyObject * value = nullptr;
  Py_ssize_t pos = 0;
  while (PyDict_Next(result, &pos, &key, &value)) {
    const char * k = PyUnicode_AsUTF8(key);
    if (!k) {continue;}
    double v = PyFloat_AsDouble(value);
    if (PyErr_Occurred()) {
      PyErr_Clear();
      continue;  // non-numeric entry (e.g. an image observation) -- skip it
    }
    obs[k] = v;
  }
  Py_DECREF(result);
  PyGILState_Release(gstate);
  return obs;
}

void SoFollowerBridge::sendAction(const std::map<std::string, double> & action_deg)
{
  PyGILState_STATE gstate = PyGILState_Ensure();

  PyObject * action_dict = PyDict_New();
  for (const auto & kv : action_deg) {
    PyObject * v = PyFloat_FromDouble(kv.second);
    PyDict_SetItemString(action_dict, kv.first.c_str(), v);
    Py_DECREF(v);
  }

  PyObject * method = PyObject_GetAttrString(robot_obj_, "send_action");
  PyObject * args = PyTuple_Pack(1, action_dict);
  PyObject * result = method ? PyObject_CallObject(method, args) : nullptr;
  Py_XDECREF(method);
  Py_DECREF(args);
  Py_DECREF(action_dict);

  if (!result) {
    std::string err = fetchPyError();
    PyGILState_Release(gstate);
    throw std::runtime_error("send_action() failed: " + err);
  }
  Py_DECREF(result);
  PyGILState_Release(gstate);
}

}  // namespace so101_hardware_interface_cpp
