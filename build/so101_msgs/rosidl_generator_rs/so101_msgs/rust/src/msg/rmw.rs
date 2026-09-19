#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "so101_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__CartesianCommand() -> *const std::ffi::c_void;
}

#[link(name = "so101_msgs__rosidl_generator_c")]
extern "C" {
    fn so101_msgs__msg__CartesianCommand__init(msg: *mut CartesianCommand) -> bool;
    fn so101_msgs__msg__CartesianCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<CartesianCommand>, size: usize) -> bool;
    fn so101_msgs__msg__CartesianCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<CartesianCommand>);
    fn so101_msgs__msg__CartesianCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<CartesianCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<CartesianCommand>) -> bool;
}

// Corresponds to so101_msgs__msg__CartesianCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Output of ANY input node (keyboard today, ArUco/IMU later), input of the controller.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CartesianCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// "velocity" or "pose"   (start with "velocity" for keyboard)
    pub mode: rosidl_runtime_rs::String,

    /// linear part used; angular part ignored (translation-only control)
    pub velocity: geometry_msgs::msg::rmw::Twist,

    /// only used if mode == "pose"
    pub target_pose: geometry_msgs::msg::rmw::Pose,

    /// false if the input node itself detected bad/stale data
    pub valid: bool,

}



impl Default for CartesianCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !so101_msgs__msg__CartesianCommand__init(&mut msg as *mut _) {
        panic!("Call to so101_msgs__msg__CartesianCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for CartesianCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__CartesianCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__CartesianCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__CartesianCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for CartesianCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for CartesianCommand where Self: Sized {
  const TYPE_NAME: &'static str = "so101_msgs/msg/CartesianCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__CartesianCommand() }
  }
}


#[link(name = "so101_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__JointVelocityCommand() -> *const std::ffi::c_void;
}

#[link(name = "so101_msgs__rosidl_generator_c")]
extern "C" {
    fn so101_msgs__msg__JointVelocityCommand__init(msg: *mut JointVelocityCommand) -> bool;
    fn so101_msgs__msg__JointVelocityCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<JointVelocityCommand>, size: usize) -> bool;
    fn so101_msgs__msg__JointVelocityCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<JointVelocityCommand>);
    fn so101_msgs__msg__JointVelocityCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<JointVelocityCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<JointVelocityCommand>) -> bool;
}

// Corresponds to so101_msgs__msg__JointVelocityCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// For the directly-teleoperated wrist joints j4/j5.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct JointVelocityCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// ["j4","j5"]
    pub joint_names: rosidl_runtime_rs::Sequence<rosidl_runtime_rs::String>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub velocities: rosidl_runtime_rs::Sequence<f64>,

}



impl Default for JointVelocityCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !so101_msgs__msg__JointVelocityCommand__init(&mut msg as *mut _) {
        panic!("Call to so101_msgs__msg__JointVelocityCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for JointVelocityCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__JointVelocityCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__JointVelocityCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__JointVelocityCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for JointVelocityCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for JointVelocityCommand where Self: Sized {
  const TYPE_NAME: &'static str = "so101_msgs/msg/JointVelocityCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__JointVelocityCommand() }
  }
}


#[link(name = "so101_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__JointCommand() -> *const std::ffi::c_void;
}

#[link(name = "so101_msgs__rosidl_generator_c")]
extern "C" {
    fn so101_msgs__msg__JointCommand__init(msg: *mut JointCommand) -> bool;
    fn so101_msgs__msg__JointCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<JointCommand>, size: usize) -> bool;
    fn so101_msgs__msg__JointCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<JointCommand>);
    fn so101_msgs__msg__JointCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<JointCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<JointCommand>) -> bool;
}

// Corresponds to so101_msgs__msg__JointCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Full 6-joint output of controller / input of safety gate / input of hardware interface.
/// Real joint names (not the earlier j1..j5 placeholder), kinematic-chain order:
///   ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
/// gripper is direct-teleop passthrough, same as wrist_flex/wrist_roll -- not IK-controlled.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct JointCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
    pub joint_names: rosidl_runtime_rs::Sequence<rosidl_runtime_rs::String>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub velocities: rosidl_runtime_rs::Sequence<f64>,

    /// NORMAL or FROZEN_TRANSLATION
    pub control_mode: u8,

}

impl JointCommand {

    // This constant is not documented.
    #[allow(missing_docs)]
    pub const NORMAL: u8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const FROZEN_TRANSLATION: u8 = 1;

}


impl Default for JointCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !so101_msgs__msg__JointCommand__init(&mut msg as *mut _) {
        panic!("Call to so101_msgs__msg__JointCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for JointCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__JointCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__JointCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__JointCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for JointCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for JointCommand where Self: Sized {
  const TYPE_NAME: &'static str = "so101_msgs/msg/JointCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__JointCommand() }
  }
}


#[link(name = "so101_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__ControllerStatus() -> *const std::ffi::c_void;
}

#[link(name = "so101_msgs__rosidl_generator_c")]
extern "C" {
    fn so101_msgs__msg__ControllerStatus__init(msg: *mut ControllerStatus) -> bool;
    fn so101_msgs__msg__ControllerStatus__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ControllerStatus>, size: usize) -> bool;
    fn so101_msgs__msg__ControllerStatus__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ControllerStatus>);
    fn so101_msgs__msg__ControllerStatus__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ControllerStatus>, out_seq: *mut rosidl_runtime_rs::Sequence<ControllerStatus>) -> bool;
}

// Corresponds to so101_msgs__msg__ControllerStatus
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Richer, structured companion to the plain diagnostic_msgs/DiagnosticArray stream.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ControllerStatus {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub in_singularity: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub manipulability_index: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub near_joint_limit: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub self_collision_detected: bool,

    /// empty string if healthy
    pub last_error: rosidl_runtime_rs::String,

}



impl Default for ControllerStatus {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !so101_msgs__msg__ControllerStatus__init(&mut msg as *mut _) {
        panic!("Call to so101_msgs__msg__ControllerStatus__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ControllerStatus {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__ControllerStatus__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__ControllerStatus__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { so101_msgs__msg__ControllerStatus__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ControllerStatus {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ControllerStatus where Self: Sized {
  const TYPE_NAME: &'static str = "so101_msgs/msg/ControllerStatus";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__so101_msgs__msg__ControllerStatus() }
  }
}


