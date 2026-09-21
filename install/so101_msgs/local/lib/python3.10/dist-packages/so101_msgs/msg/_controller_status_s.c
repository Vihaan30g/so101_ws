// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from so101_msgs:msg/ControllerStatus.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "so101_msgs/msg/detail/controller_status__struct.h"
#include "so101_msgs/msg/detail/controller_status__functions.h"

#include "rosidl_runtime_c/string.h"
#include "rosidl_runtime_c/string_functions.h"

ROSIDL_GENERATOR_C_IMPORT
bool std_msgs__msg__header__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * std_msgs__msg__header__convert_to_py(void * raw_ros_message);

ROSIDL_GENERATOR_C_EXPORT
bool so101_msgs__msg__controller_status__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[51];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("so101_msgs.msg._controller_status.ControllerStatus", full_classname_dest, 50) == 0);
  }
  so101_msgs__msg__ControllerStatus * ros_message = _ros_message;
  {  // header
    PyObject * field = PyObject_GetAttrString(_pymsg, "header");
    if (!field) {
      return false;
    }
    if (!std_msgs__msg__header__convert_from_py(field, &ros_message->header)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }
  {  // in_singularity
    PyObject * field = PyObject_GetAttrString(_pymsg, "in_singularity");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->in_singularity = (Py_True == field);
    Py_DECREF(field);
  }
  {  // manipulability_index
    PyObject * field = PyObject_GetAttrString(_pymsg, "manipulability_index");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->manipulability_index = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // near_joint_limit
    PyObject * field = PyObject_GetAttrString(_pymsg, "near_joint_limit");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->near_joint_limit = (Py_True == field);
    Py_DECREF(field);
  }
  {  // self_collision_detected
    PyObject * field = PyObject_GetAttrString(_pymsg, "self_collision_detected");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->self_collision_detected = (Py_True == field);
    Py_DECREF(field);
  }
  {  // last_error
    PyObject * field = PyObject_GetAttrString(_pymsg, "last_error");
    if (!field) {
      return false;
    }
    assert(PyUnicode_Check(field));
    PyObject * encoded_field = PyUnicode_AsUTF8String(field);
    if (!encoded_field) {
      Py_DECREF(field);
      return false;
    }
    rosidl_runtime_c__String__assign(&ros_message->last_error, PyBytes_AS_STRING(encoded_field));
    Py_DECREF(encoded_field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * so101_msgs__msg__controller_status__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ControllerStatus */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("so101_msgs.msg._controller_status");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ControllerStatus");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  so101_msgs__msg__ControllerStatus * ros_message = (so101_msgs__msg__ControllerStatus *)raw_ros_message;
  {  // header
    PyObject * field = NULL;
    field = std_msgs__msg__header__convert_to_py(&ros_message->header);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "header", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // in_singularity
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->in_singularity ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "in_singularity", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // manipulability_index
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->manipulability_index);
    {
      int rc = PyObject_SetAttrString(_pymessage, "manipulability_index", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // near_joint_limit
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->near_joint_limit ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "near_joint_limit", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // self_collision_detected
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->self_collision_detected ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "self_collision_detected", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // last_error
    PyObject * field = NULL;
    field = PyUnicode_DecodeUTF8(
      ros_message->last_error.data,
      strlen(ros_message->last_error.data),
      "replace");
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "last_error", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
