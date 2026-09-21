# generated from rosidl_generator_py/resource/_idl.py.em
# with input from so101_msgs:msg/ControllerStatus.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ControllerStatus(type):
    """Metaclass of message 'ControllerStatus'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('so101_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'so101_msgs.msg.ControllerStatus')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__controller_status
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__controller_status
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__controller_status
            cls._TYPE_SUPPORT = module.type_support_msg__msg__controller_status
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__controller_status

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ControllerStatus(metaclass=Metaclass_ControllerStatus):
    """Message class 'ControllerStatus'."""

    __slots__ = [
        '_header',
        '_in_singularity',
        '_manipulability_index',
        '_near_joint_limit',
        '_self_collision_detected',
        '_last_error',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'in_singularity': 'boolean',
        'manipulability_index': 'double',
        'near_joint_limit': 'boolean',
        'self_collision_detected': 'boolean',
        'last_error': 'string',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.in_singularity = kwargs.get('in_singularity', bool())
        self.manipulability_index = kwargs.get('manipulability_index', float())
        self.near_joint_limit = kwargs.get('near_joint_limit', bool())
        self.self_collision_detected = kwargs.get('self_collision_detected', bool())
        self.last_error = kwargs.get('last_error', str())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.in_singularity != other.in_singularity:
            return False
        if self.manipulability_index != other.manipulability_index:
            return False
        if self.near_joint_limit != other.near_joint_limit:
            return False
        if self.self_collision_detected != other.self_collision_detected:
            return False
        if self.last_error != other.last_error:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def in_singularity(self):
        """Message field 'in_singularity'."""
        return self._in_singularity

    @in_singularity.setter
    def in_singularity(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'in_singularity' field must be of type 'bool'"
        self._in_singularity = value

    @builtins.property
    def manipulability_index(self):
        """Message field 'manipulability_index'."""
        return self._manipulability_index

    @manipulability_index.setter
    def manipulability_index(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'manipulability_index' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'manipulability_index' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._manipulability_index = value

    @builtins.property
    def near_joint_limit(self):
        """Message field 'near_joint_limit'."""
        return self._near_joint_limit

    @near_joint_limit.setter
    def near_joint_limit(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'near_joint_limit' field must be of type 'bool'"
        self._near_joint_limit = value

    @builtins.property
    def self_collision_detected(self):
        """Message field 'self_collision_detected'."""
        return self._self_collision_detected

    @self_collision_detected.setter
    def self_collision_detected(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'self_collision_detected' field must be of type 'bool'"
        self._self_collision_detected = value

    @builtins.property
    def last_error(self):
        """Message field 'last_error'."""
        return self._last_error

    @last_error.setter
    def last_error(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'last_error' field must be of type 'str'"
        self._last_error = value
