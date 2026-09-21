#pragma once
// so101_kinematics.hpp
//
// Shared Pinocchio-based kinematics core for the SO-101 arm.
// Linked into BOTH cartesian_controller_node and safety_gate_node so they
// can never disagree about where the arm's links physically are
// (see design doc §4).
//
// STATUS: FK, Jacobians, manipulability, and DLS IK solve are fully
// implemented. Self-collision checking is a stub — see the TODO block
// near checkSelfCollision() and HANDOFF.md at the repo root.

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <Eigen/Dense>

#include <pinocchio/multibody/data.hpp>
#include <pinocchio/multibody/geometry.hpp>
#include <pinocchio/multibody/model.hpp>

namespace so101_kinematics
{

struct KinematicsConfig
{
  std::string urdf_path;
  std::string end_effector_frame{"gripper_tip"};
  std::vector<std::string> translational_joints{"j1", "j2", "j3"};
  std::vector<std::string> wrist_joints{"j4", "j5"};

  // DLS damping: lambda^2 = lambda0^2 * max(0, 1 - w/w_threshold)
  // Far from singularities (w >= w_threshold) lambda == 0 (near-exact tracking).
  double dls_lambda0{0.05};
  double manipulability_threshold{0.02};
};

struct IKResult
{
  Eigen::VectorXd joint_velocities;      // size == translational_joints.size()
  double manipulability{0.0};            // w = sqrt(det(J J^T))
  double lambda_used{0.0};
  bool near_singularity{false};
};

class So101Kinematics
{
public:
  explicit So101Kinematics(const KinematicsConfig & config);

  // --- Core kinematics (fully implemented) ---

  // Runs forward kinematics + frame placement update, returns EE position
  // (world frame, LOCAL_WORLD_ALIGNED translation part).
  Eigen::Vector3d computeEndEffectorPosition(const Eigen::VectorXd & q);

  // Full 6 x nv spatial Jacobian at the EE frame, LOCAL_WORLD_ALIGNED.
  Eigen::MatrixXd computeFullJacobian(const Eigen::VectorXd & q);

  // 3 x K reduced *translational* Jacobian, columns restricted to the
  // configured translational_joints (K = translational_joints.size()).
  // This is the deliberate "only 3 joints are under Cartesian control"
  // sub-block described in the design doc §4.
  Eigen::MatrixXd computeReducedTranslationalJacobian(const Eigen::VectorXd & q);

  // Manipulability index w = sqrt(det(J J^T)). Works for any J you pass in
  // (call with the reduced Jacobian for the controller's singularity check).
  double computeManipulability(const Eigen::MatrixXd & J) const;

  // Damped least-squares IK solve for the translational joints only:
  //   theta_dot = J^T (J J^T + lambda^2 I)^-1 v_cmd
  // with lambda ramped adaptively from the manipulability index.
  // Does NOT touch j4/j5 — those are direct teleop passthrough, handled
  // by the controller node, not this library.
  IKResult solveDLS(const Eigen::VectorXd & q, const Eigen::Vector3d & v_cmd);

  // --- Limits (read straight from the URDF via the Pinocchio model) ---
  const Eigen::VectorXd & lowerPositionLimits() const { return model_.lowerPositionLimit; }
  const Eigen::VectorXd & upperPositionLimits() const { return model_.upperPositionLimit; }
  const Eigen::VectorXd & velocityLimits() const { return model_.velocityLimit; }

  // --- Index / bookkeeping helpers ---
  // Config-space (q) index for each configured translational joint, in order.
  const std::vector<int> & translationalJointConfigIndices() const { return translational_q_idx_; }
  // Tangent-space (v) index for each configured translational joint, in order.
  // Use this one when indexing into Jacobian columns / velocity vectors.
  const std::vector<int> & translationalJointVelocityIndices() const { return translational_v_idx_; }

  int nq() const { return model_.nq; }
  int nv() const { return model_.nv; }
  const std::vector<std::string> & jointOrder() const { return joint_order_; }

  pinocchio::Model & model() { return model_; }
  pinocchio::Data & data() { return data_; }

  // --- Self-collision (STUB — see HANDOFF.md) ---
  //
  // Intended contract once implemented:
  //   loadCollisionModel() parses <collision> geometry from the URDF via
  //   pinocchio::GeometryModel, optionally removes adjacent-link pairs
  //   using an SRDF, and stores collision pairs.
  //   checkSelfCollision(q) runs pinocchio::computeCollisions() over those
  //   pairs at the *commanded next configuration* and returns true if any
  //   pair is in contact.
  //
  // NOT implemented yet. Currently:
  //   - loadCollisionModel() throws std::runtime_error("not implemented").
  //   - checkSelfCollision() always returns false and logs nothing — the
  //     safety gate must NOT rely on this for real hardware safety until
  //     this is completed. Rely on joint-limit and velocity-limit checks
  //     (which ARE real) until then.
  bool loadCollisionModel(const std::string & srdf_path = "");
  bool checkSelfCollision(const Eigen::VectorXd & q);

private:
  KinematicsConfig config_;
  pinocchio::Model model_;
  pinocchio::Data data_;
  pinocchio::FrameIndex ee_frame_id_{0};

  std::vector<int> translational_q_idx_;
  std::vector<int> translational_v_idx_;
  std::vector<std::string> joint_order_;

  std::unique_ptr<pinocchio::GeometryModel> geom_model_;
  std::unique_ptr<pinocchio::GeometryData> geom_data_;
  bool collision_model_loaded_{false};
};

}  // namespace so101_kinematics
