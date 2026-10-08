#include <cmath>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <Eigen/Geometry>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/task_constructor/solvers/pipeline_planner.h>
#include <moveit/task_constructor/solvers/joint_interpolation.h>
#include <moveit/task_constructor/stages/compute_ik.h>
#include <moveit/task_constructor/stages/connect.h>
#include <moveit/task_constructor/stages/current_state.h>
#include <moveit/task_constructor/stages/generate_grasp_pose.h>
#include <moveit/task_constructor/stages/generate_place_pose.h>
#include <moveit/task_constructor/stages/modify_planning_scene.h>
#include <moveit/task_constructor/stages/move_to.h>
#include <moveit/task_constructor/task.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <moveit_msgs/msg/move_it_error_codes.hpp>
#include <rclcpp/rclcpp.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>

namespace mtc = moveit::task_constructor;

namespace
{
constexpr char kObjectId[] = "pick_place_cylinder";
constexpr char kArmGroup[] = "body_grp";
constexpr char kHandGroup[] = "eef_grp";
constexpr char kEndEffector[] = "eef";
constexpr char kHandFrame[] = "gripper_frame_link";

geometry_msgs::msg::Pose makePose(double x, double y, double z)
{
  geometry_msgs::msg::Pose pose;
  pose.position.x = x;
  pose.position.y = y;
  pose.position.z = z;
  pose.orientation.w = 1.0;
  return pose;
}

bool addCylinder(
  moveit::planning_interface::PlanningSceneInterface & planning_scene,
  const rclcpp::Node::SharedPtr & node)
{
  const auto x = node->get_parameter_or<double>("object_x", -0.18);
  const auto y = node->get_parameter_or<double>("object_y", 0.08);
  const auto z = node->get_parameter_or<double>("object_z", 0.045);
  const auto height = node->get_parameter_or<double>("cylinder_height", 0.08);
  const auto radius = node->get_parameter_or<double>("cylinder_radius", 0.022);

  moveit_msgs::msg::CollisionObject object;
  object.id = kObjectId;
  object.header.frame_id = "world";

  shape_msgs::msg::SolidPrimitive cylinder;
  cylinder.type = shape_msgs::msg::SolidPrimitive::CYLINDER;
  cylinder.dimensions = {height, radius};
  object.primitives.push_back(cylinder);
  object.primitive_poses.push_back(makePose(x, y, z));
  object.operation = moveit_msgs::msg::CollisionObject::ADD;

  if (!planning_scene.applyCollisionObject(object)) {
    RCLCPP_ERROR(node->get_logger(), "Failed to add the cylinder to the planning scene");
    return false;
  }
  RCLCPP_INFO(
    node->get_logger(), "Added cylinder '%s' at (%.3f, %.3f, %.3f)",
    kObjectId, x, y, z);
  return true;
}

bool buildAndExecuteTask(const rclcpp::Node::SharedPtr & node)
{
  moveit::planning_interface::PlanningSceneInterface planning_scene;
  if (!addCylinder(planning_scene, node)) {
    return false;
  }

  const auto place_x = node->get_parameter_or<double>("place_x", -0.18);
  const auto place_y = node->get_parameter_or<double>("place_y", -0.08);
  const auto place_z = node->get_parameter_or<double>("place_z", 0.045);

  mtc::Task task("so101_pick_place");
  task.loadRobotModel(node);
  task.setProperty("group", kArmGroup);
  task.setProperty("eef", kEndEffector);
  task.setProperty("hand", kHandGroup);
  task.setProperty("hand_grasping_frame", "gripper_link");
  task.setProperty("ik_frame", kHandFrame);
  const auto * hand_group = task.getRobotModel()->getJointModelGroup(kHandGroup);
  if (hand_group == nullptr) {
    RCLCPP_ERROR(node->get_logger(), "Robot model has no '%s' group", kHandGroup);
    return false;
  }
  auto hand_collision_links = hand_group->getLinkModelNamesWithCollisionGeometry();
  hand_collision_links.emplace_back("gripper_link");

  auto sampling_planner = std::make_shared<mtc::solvers::PipelinePlanner>(node);
  auto joint_interpolation =
    std::make_shared<mtc::solvers::JointInterpolationPlanner>();
  // This transform maps the SO-101's zero-state tool pose to a vertical object
  // pose, keeping the grasp reachable for the arm's five-DOF kinematic chain.
  Eigen::Isometry3d grasp_frame = Eigen::Isometry3d::Identity();
  const Eigen::Quaterniond grasp_orientation(
    0.7069004666, -0.0172064748, -0.7068941641, -0.0172136070);
  grasp_frame.linear() = grasp_orientation.toRotationMatrix();
  grasp_frame.translation().z() = 0.04;

  mtc::Stage * open_hand_stage = nullptr;
  task.add(std::make_unique<mtc::stages::CurrentState>("current state"));
  {
    auto stage = std::make_unique<mtc::stages::MoveTo>(
      "open gripper", joint_interpolation);
    stage->setGroup(kHandGroup);
    stage->setGoal("full_open");
    open_hand_stage = stage.get();
    task.add(std::move(stage));
  }

  {
    auto stage = std::make_unique<mtc::stages::Connect>(
      "move to pick", mtc::stages::Connect::GroupPlannerVector{
        {kArmGroup, sampling_planner}});
    stage->setTimeout(5.0);
    stage->properties().configureInitFrom(mtc::Stage::PARENT);
    task.add(std::move(stage));
  }

  mtc::Stage * attach_stage = nullptr;
  {
    auto pick = std::make_unique<mtc::SerialContainer>("pick cylinder");
    task.properties().exposeTo(
      pick->properties(), {"eef", "hand", "group", "ik_frame"});
    pick->properties().configureInitFrom(
      mtc::Stage::PARENT, {"eef", "hand", "group", "ik_frame"});

    {
      auto stage = std::make_unique<mtc::stages::ModifyPlanningScene>(
        "allow gripper contact");
      stage->allowCollisions(kObjectId, hand_collision_links, true);
      pick->insert(std::move(stage));
    }

    {
      auto grasp = std::make_unique<mtc::stages::GenerateGraspPose>(
        "generate grasp poses");
      grasp->properties().configureInitFrom(mtc::Stage::PARENT);
      grasp->properties().set("marker_ns", "grasp_pose");
      grasp->setPreGraspPose("full_open");
      grasp->setObject(kObjectId);
      grasp->setAngleDelta(M_PI / 6.0);
      grasp->setMonitoredStage(open_hand_stage);

      auto compute_ik = std::make_unique<mtc::stages::ComputeIK>(
        "grasp pose IK", std::move(grasp));
      compute_ik->setMaxIKSolutions(8);
      compute_ik->setMinSolutionDistance(0.2);
      compute_ik->setIKFrame(grasp_frame, kHandFrame);
      compute_ik->properties().configureInitFrom(
        mtc::Stage::PARENT, {"eef", "group"});
      compute_ik->properties().configureInitFrom(
        mtc::Stage::INTERFACE, {"target_pose"});
      pick->insert(std::move(compute_ik));
    }

    {
      auto stage = std::make_unique<mtc::stages::MoveTo>(
        "close gripper", joint_interpolation);
      stage->setGroup(kHandGroup);
      stage->setGoal("complete_close");
      pick->insert(std::move(stage));
    }

    {
      auto stage = std::make_unique<mtc::stages::ModifyPlanningScene>(
        "attach cylinder");
      stage->attachObject(kObjectId, "gripper_link");
      attach_stage = stage.get();
      pick->insert(std::move(stage));
    }

    task.add(std::move(pick));
  }

  {
    auto stage = std::make_unique<mtc::stages::Connect>(
      "move to place", mtc::stages::Connect::GroupPlannerVector{
        {kArmGroup, sampling_planner}});
    stage->setTimeout(5.0);
    stage->properties().configureInitFrom(mtc::Stage::PARENT);
    task.add(std::move(stage));
  }

  {
    auto place = std::make_unique<mtc::SerialContainer>("place cylinder");
    task.properties().exposeTo(
      place->properties(), {"eef", "hand", "group", "ik_frame"});
    place->properties().configureInitFrom(
      mtc::Stage::PARENT, {"eef", "hand", "group", "ik_frame"});

    auto target = std::make_unique<mtc::stages::GeneratePlacePose>(
      "generate place pose");
    target->properties().configureInitFrom(mtc::Stage::PARENT, {"ik_frame"});
    target->properties().set("marker_ns", "place_pose");
    target->setObject(kObjectId);
    geometry_msgs::msg::PoseStamped place_pose;
    place_pose.header.frame_id = "world";
    place_pose.pose = makePose(place_x, place_y, place_z);
    target->setPose(place_pose);
    target->setMonitoredStage(attach_stage);

    auto place_ik = std::make_unique<mtc::stages::ComputeIK>(
      "place pose IK", std::move(target));
    place_ik->setMaxIKSolutions(4);
    place_ik->setMinSolutionDistance(0.2);
    place_ik->setIKFrame(grasp_frame, kHandFrame);
    place_ik->properties().configureInitFrom(
      mtc::Stage::PARENT, {"eef", "group"});
    place_ik->properties().configureInitFrom(
      mtc::Stage::INTERFACE, {"target_pose"});
    place->insert(std::move(place_ik));

    {
      auto stage = std::make_unique<mtc::stages::MoveTo>(
        "open gripper to release", joint_interpolation);
      stage->setGroup(kHandGroup);
      stage->setGoal("full_open");
      place->insert(std::move(stage));
    }

    {
      auto stage = std::make_unique<mtc::stages::ModifyPlanningScene>(
        "allow cylinder to leave gripper");
      stage->allowCollisions(kObjectId, hand_collision_links, false);
      place->insert(std::move(stage));
    }

    {
      auto stage = std::make_unique<mtc::stages::ModifyPlanningScene>(
        "detach cylinder");
      stage->detachObject(kObjectId, "gripper_link");
      place->insert(std::move(stage));
    }

    task.add(std::move(place));
  }

  try {
    task.init();
  } catch (const mtc::InitStageException & exception) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "Task initialization failed: " << exception);
    return false;
  }

  task.introspection().publishTaskDescription();
  RCLCPP_INFO(node->get_logger(), "Planning the SO-101 pick-and-place task");
  if (!task.plan(1)) {
    RCLCPP_ERROR(node->get_logger(), "No complete pick-and-place solution was found");
    task.explainFailure();
    return false;
  }

  task.publishAllSolutions(false);
  const auto result = task.execute(*task.solutions().front());
  if (result.val != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
    RCLCPP_ERROR(
      node->get_logger(), "Task execution failed with MoveIt error code %d", result.val);
    return false;
  }

  RCLCPP_INFO(node->get_logger(), "Pick-and-place task completed successfully");
  return true;
}
}  // namespace

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions options;
  options.automatically_declare_parameters_from_overrides(true);
  auto node = std::make_shared<rclcpp::Node>("so101_pick_place_task", options);
  std::thread spin_thread([node]() {rclcpp::spin(node);});

  const bool success = buildAndExecuteTask(node);
  if (!success) {
    rclcpp::shutdown();
    spin_thread.join();
    return 1;
  }

  spin_thread.join();
  return 0;
}
