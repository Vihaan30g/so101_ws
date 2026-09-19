#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to so101_msgs__msg__CartesianCommand
/// Output of ANY input node (keyboard today, ArUco/IMU later), input of the controller.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CartesianCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// "velocity" or "pose"   (start with "velocity" for keyboard)
    pub mode: std::string::String,

    /// linear part used; angular part ignored (translation-only control)
    pub velocity: geometry_msgs::msg::Twist,

    /// only used if mode == "pose"
    pub target_pose: geometry_msgs::msg::Pose,

    /// false if the input node itself detected bad/stale data
    pub valid: bool,

}



impl Default for CartesianCommand {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::CartesianCommand::default())
  }
}

impl rosidl_runtime_rs::Message for CartesianCommand {
  type RmwMsg = super::msg::rmw::CartesianCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        mode: msg.mode.as_str().into(),
        velocity: geometry_msgs::msg::Twist::into_rmw_message(std::borrow::Cow::Owned(msg.velocity)).into_owned(),
        target_pose: geometry_msgs::msg::Pose::into_rmw_message(std::borrow::Cow::Owned(msg.target_pose)).into_owned(),
        valid: msg.valid,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        mode: msg.mode.as_str().into(),
        velocity: geometry_msgs::msg::Twist::into_rmw_message(std::borrow::Cow::Borrowed(&msg.velocity)).into_owned(),
        target_pose: geometry_msgs::msg::Pose::into_rmw_message(std::borrow::Cow::Borrowed(&msg.target_pose)).into_owned(),
      valid: msg.valid,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      mode: msg.mode.to_string(),
      velocity: geometry_msgs::msg::Twist::from_rmw_message(msg.velocity),
      target_pose: geometry_msgs::msg::Pose::from_rmw_message(msg.target_pose),
      valid: msg.valid,
    }
  }
}


// Corresponds to so101_msgs__msg__JointVelocityCommand
/// For the directly-teleoperated wrist joints j4/j5.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct JointVelocityCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// ["j4","j5"]
    pub joint_names: Vec<std::string::String>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub velocities: Vec<f64>,

}



impl Default for JointVelocityCommand {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::JointVelocityCommand::default())
  }
}

impl rosidl_runtime_rs::Message for JointVelocityCommand {
  type RmwMsg = super::msg::rmw::JointVelocityCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        joint_names: msg.joint_names
          .into_iter()
          .map(|elem| elem.as_str().into())
          .collect(),
        velocities: msg.velocities.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        joint_names: msg.joint_names
          .iter()
          .map(|elem| elem.as_str().into())
          .collect(),
        velocities: msg.velocities.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      joint_names: msg.joint_names
          .into_iter()
          .map(|elem| elem.to_string())
          .collect(),
      velocities: msg.velocities
          .into_iter()
          .collect(),
    }
  }
}


// Corresponds to so101_msgs__msg__JointCommand
/// Full 6-joint output of controller / input of safety gate / input of hardware interface.
/// Real joint names (not the earlier j1..j5 placeholder), kinematic-chain order:
///   ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
/// gripper is direct-teleop passthrough, same as wrist_flex/wrist_roll -- not IK-controlled.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct JointCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
    pub joint_names: Vec<std::string::String>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub velocities: Vec<f64>,

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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::JointCommand::default())
  }
}

impl rosidl_runtime_rs::Message for JointCommand {
  type RmwMsg = super::msg::rmw::JointCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        joint_names: msg.joint_names
          .into_iter()
          .map(|elem| elem.as_str().into())
          .collect(),
        velocities: msg.velocities.into(),
        control_mode: msg.control_mode,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        joint_names: msg.joint_names
          .iter()
          .map(|elem| elem.as_str().into())
          .collect(),
        velocities: msg.velocities.as_slice().into(),
      control_mode: msg.control_mode,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      joint_names: msg.joint_names
          .into_iter()
          .map(|elem| elem.to_string())
          .collect(),
      velocities: msg.velocities
          .into_iter()
          .collect(),
      control_mode: msg.control_mode,
    }
  }
}


// Corresponds to so101_msgs__msg__ControllerStatus
/// Richer, structured companion to the plain diagnostic_msgs/DiagnosticArray stream.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ControllerStatus {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


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
    pub last_error: std::string::String,

}



impl Default for ControllerStatus {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ControllerStatus::default())
  }
}

impl rosidl_runtime_rs::Message for ControllerStatus {
  type RmwMsg = super::msg::rmw::ControllerStatus;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        in_singularity: msg.in_singularity,
        manipulability_index: msg.manipulability_index,
        near_joint_limit: msg.near_joint_limit,
        self_collision_detected: msg.self_collision_detected,
        last_error: msg.last_error.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      in_singularity: msg.in_singularity,
      manipulability_index: msg.manipulability_index,
      near_joint_limit: msg.near_joint_limit,
      self_collision_detected: msg.self_collision_detected,
        last_error: msg.last_error.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      in_singularity: msg.in_singularity,
      manipulability_index: msg.manipulability_index,
      near_joint_limit: msg.near_joint_limit,
      self_collision_detected: msg.self_collision_detected,
      last_error: msg.last_error.to_string(),
    }
  }
}


