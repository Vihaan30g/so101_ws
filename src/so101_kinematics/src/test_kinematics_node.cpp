// test_kinematics_node.cpp
//
// Plain standalone sanity check (NOT an rclcpp node -- no ROS graph needed).
// Matches build-order step 1 from the design doc: "load the real URDF,
// verify FK/Jacobian against known poses" before writing any control code.
//
// Usage:
//   test_kinematics_node <path_to_urdf> [end_effector_frame_name]

#include <iostream>

#include <Eigen/Dense>

#include "so101_kinematics/so101_kinematics.hpp"

int main(int argc, char ** argv)
{
  if (argc < 2) {
    std::cerr << "Usage: test_kinematics_node <path_to_urdf> [ee_frame_name]\n";
    return 1;
  }

  so101_kinematics::KinematicsConfig cfg;
  cfg.urdf_path = argv[1];
  if (argc >= 3) {
    cfg.end_effector_frame = argv[2];
  }

  try {
    so101_kinematics::So101Kinematics kin(cfg);

    std::cout << "Loaded URDF OK: " << cfg.urdf_path << "\n";
    std::cout << "nq = " << kin.nq() << ", nv = " << kin.nv() << "\n";
    std::cout << "Joint order:\n";
    for (const auto & n : kin.jointOrder()) {
      std::cout << "  " << n << "\n";
    }

    const Eigen::VectorXd q = Eigen::VectorXd::Zero(kin.nq());

    const Eigen::Vector3d p = kin.computeEndEffectorPosition(q);
    std::cout << "\nEE position at q=0: " << p.transpose() << "\n";

    const Eigen::MatrixXd J_t = kin.computeReducedTranslationalJacobian(q);
    std::cout << "\nReduced translational Jacobian (3x" << J_t.cols()
              << ") at q=0:\n" << J_t << "\n";

    const double w = kin.computeManipulability(J_t);
    std::cout << "\nManipulability at q=0: " << w << "\n";

    const Eigen::Vector3d v_cmd(0.01, 0.0, 0.0);
    const auto ik = kin.solveDLS(q, v_cmd);
    std::cout << "\nDLS solve for v_cmd = (0.01, 0, 0) m/s:\n";
    std::cout << "  joint_velocities: " << ik.joint_velocities.transpose() << "\n";
    std::cout << "  manipulability:   " << ik.manipulability << "\n";
    std::cout << "  lambda_used:      " << ik.lambda_used << "\n";
    std::cout << "  near_singularity: " << (ik.near_singularity ? "true" : "false") << "\n";

    std::cout << "\nLower position limits: " << kin.lowerPositionLimits().transpose() << "\n";
    std::cout << "Upper position limits: " << kin.upperPositionLimits().transpose() << "\n";
    std::cout << "Velocity limits:       " << kin.velocityLimits().transpose() << "\n";

  } catch (const std::exception & e) {
    std::cerr << "Kinematics test FAILED: " << e.what() << "\n";
    return 1;
  }

  std::cout << "\nAll kinematics checks ran without throwing.\n";
  return 0;
}
