#include "so101_kinematics/so101_kinematics.hpp"

#include <algorithm>
#include <cmath>

#include <pinocchio/algorithm/frames.hpp>
#include <pinocchio/algorithm/jacobian.hpp>
#include <pinocchio/algorithm/kinematics.hpp>
#include <pinocchio/parsers/urdf.hpp>

namespace so101_kinematics
{

So101Kinematics::So101Kinematics(const KinematicsConfig & config)
: config_(config), model_(), data_()
{
  if (config_.urdf_path.empty()) {
    throw std::runtime_error("so101_kinematics: urdf_path is empty (check robot_params.yaml)");
  }

  // NOTE: pinocchio::urdf::buildModel throws its own descriptive exception
  // (pinocchio::urdf::ParsingException-derived) if the file is missing or
  // malformed. We deliberately do not swallow it here -- the caller (the
  // controller/safety node constructor) is expected to catch, log via
  // RCLCPP_ERROR, and refuse to come up healthy per the design doc's
  // "fail loud, fail to a safe state" philosophy (§10).
  pinocchio::urdf::buildModel(config_.urdf_path, model_);
  data_ = pinocchio::Data(model_);

  if (!model_.existFrame(config_.end_effector_frame)) {
    throw std::runtime_error(
      "so101_kinematics: end-effector frame '" + config_.end_effector_frame +
      "' not found in URDF. Check 'end_effector_frame' in robot_params.yaml "
      "against the actual frame/link names in so101.urdf.");
  }
  ee_frame_id_ = model_.getFrameId(config_.end_effector_frame);

  // Full ordered joint name list (nv-space order), skipping the implicit
  // 'universe' joint at index 0.
  joint_order_.reserve(static_cast<size_t>(model_.njoints - 1));
  for (pinocchio::JointIndex j = 1; j < static_cast<pinocchio::JointIndex>(model_.njoints); ++j) {
    joint_order_.push_back(model_.names[j]);
  }

  translational_q_idx_.clear();
  translational_v_idx_.clear();
  translational_q_idx_.reserve(config_.translational_joints.size());
  translational_v_idx_.reserve(config_.translational_joints.size());

  for (const auto & jname : config_.translational_joints) {
    if (!model_.existJointName(jname)) {
      throw std::runtime_error(
        "so101_kinematics: translational joint '" + jname +
        "' from robot_params.yaml not found in URDF.");
    }
    const auto jid = model_.getJointId(jname);
    translational_q_idx_.push_back(model_.joints[jid].idx_q());
    translational_v_idx_.push_back(model_.joints[jid].idx_v());
  }
}

Eigen::Vector3d So101Kinematics::computeEndEffectorPosition(const Eigen::VectorXd & q)
{
  pinocchio::forwardKinematics(model_, data_, q);
  pinocchio::updateFramePlacements(model_, data_);
  return data_.oMf[ee_frame_id_].translation();
}

Eigen::MatrixXd So101Kinematics::computeFullJacobian(const Eigen::VectorXd & q)
{
  pinocchio::computeJointJacobians(model_, data_, q);
  pinocchio::updateFramePlacements(model_, data_);

  Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, model_.nv);
  pinocchio::getFrameJacobian(
    model_, data_, ee_frame_id_, pinocchio::LOCAL_WORLD_ALIGNED, J);
  return J;
}

Eigen::MatrixXd So101Kinematics::computeReducedTranslationalJacobian(const Eigen::VectorXd & q)
{
  const Eigen::MatrixXd J_full = computeFullJacobian(q);
  const Eigen::MatrixXd J_lin = J_full.topRows(3);  // 3 x nv, linear velocity rows only

  Eigen::MatrixXd J_t(3, static_cast<Eigen::Index>(translational_v_idx_.size()));
  for (size_t i = 0; i < translational_v_idx_.size(); ++i) {
    J_t.col(static_cast<Eigen::Index>(i)) = J_lin.col(translational_v_idx_[i]);
  }
  return J_t;
}

double So101Kinematics::computeManipulability(const Eigen::MatrixXd & J) const
{
  const Eigen::MatrixXd JJt = J * J.transpose();
  const double det = JJt.determinant();
  // Guard against tiny negative values from floating point noise at det ~ 0.
  return det > 0.0 ? std::sqrt(det) : 0.0;
}

IKResult So101Kinematics::solveDLS(const Eigen::VectorXd & q, const Eigen::Vector3d & v_cmd)
{
  IKResult result;

  const Eigen::MatrixXd J_t = computeReducedTranslationalJacobian(q);  // 3 x K
  const double w = computeManipulability(J_t);
  result.manipulability = w;

  const double lambda0 = config_.dls_lambda0;
  const double w_threshold = config_.manipulability_threshold;

  double lambda_sq = 0.0;
  if (w_threshold > 0.0 && w < w_threshold) {
    lambda_sq = lambda0 * lambda0 * std::max(0.0, 1.0 - w / w_threshold);
    result.near_singularity = true;
  }
  result.lambda_used = std::sqrt(lambda_sq);

  const Eigen::Matrix3d JJt = J_t * J_t.transpose();
  const Eigen::Matrix3d damped = JJt + lambda_sq * Eigen::Matrix3d::Identity();

  // ldlt() is a good default for a symmetric 3x3 that's guaranteed PSD
  // (JJt is always PSD; + lambda^2*I keeps it well-conditioned even at w=0).
  const Eigen::Vector3d y = damped.ldlt().solve(v_cmd);
  result.joint_velocities = J_t.transpose() * y;  // K-vector, theta_dot

  return result;
}

// ---------------------------------------------------------------------
// Self-collision: STUB. See so101_kinematics.hpp doc comment and
// HANDOFF.md for what's required to finish this. Deliberately left
// unimplemented for this drop so the FK/Jacobian/DLS core (the part
// everything else depends on) could be finished and verified first.
// ---------------------------------------------------------------------

bool So101Kinematics::loadCollisionModel(const std::string & /*srdf_path*/)
{
  // TODO(next session):
  //   1. pinocchio::GeometryModel geom_model;
  //      pinocchio::urdf::buildGeom(model_, config_.urdf_path, pinocchio::COLLISION, geom_model);
  //   2. geom_model.addAllCollisionPairs();
  //   3. If an SRDF is provided, call
  //      pinocchio::srdf::removeCollisionPairs(model_, geom_model, srdf_path)
  //      to drop known-adjacent-link false positives (e.g. j1/j2 housings
  //      that always touch at the joint).
  //   4. Store into geom_model_ / geom_data_, set collision_model_loaded_ = true.
  throw std::runtime_error(
    "So101Kinematics::loadCollisionModel: not implemented yet. "
    "See HANDOFF.md, task 'safety_gate_node' and 'so101_kinematics collision "
    "support' before wiring self-collision checks into the safety gate.");
}

bool So101Kinematics::checkSelfCollision(const Eigen::VectorXd & /*q*/)
{
  // Deliberately conservative: returns "no collision detected" rather than
  // silently pretending to check. Do NOT treat this as a real safety layer
  // until loadCollisionModel()/this function are actually implemented --
  // rely on joint position/velocity limit checks in so101_safety in the
  // meantime, and keep a human supervising the real arm at all times.
  return false;
}

}  // namespace so101_kinematics
