#include "ik_solver.hpp"

namespace rix {
namespace rdf {

IKSolver::IKSolver(Tree &tree) : tree(tree) {}

rix::msg::sensor::JS IKSolver::solve(const std::string &link_name, const Eigen::Affine3d &goal,
                                     const rix::msg::sensor::JS &initial_guess, double step_scale, double tolerance,
                                     uint32_t max_iterations, bool &converged) {
    /**
     * TODO: Implement the inverse kinematics solver. This function should 
     * return the joint state that converged to the goal pose. The function
     * should also set the converged flag to true if the solution converged
     * within the specified tolerance and maximum iterations.
     */
    
     FKSolver fk_solver(tree);
     auto current_js = initial_guess;
     converged = false;
 
     for (uint32_t i = 0; i < max_iterations; ++i) {
         // Extract joint values (assume each JointState has a 'position' field)
         std::vector<double> joint_values;
         for (const auto &js : current_js.joint_states) {
             joint_values.push_back(js.position);
         }
 
         // Forward kinematics to get current end-effector pose
         std::unordered_map<std::string, double> joint_position_map;
for (const auto &joint : current_js.joint_states) {
    joint_position_map[joint.name] = joint.position;
}

Eigen::Affine3d current_pose = fk_solver.solve(joint_position_map, link_name);
 
         // Compute error between current pose and goal pose
         Eigen::Vector3d pos_err = goal.translation() - current_pose.translation();
         Eigen::AngleAxisd rot_err(goal.rotation() * current_pose.rotation().transpose());
         Eigen::VectorXd error(6);
         error.head<3>() = pos_err;
         error.tail<3>() = rot_err.angle() * rot_err.axis();
 
         if (error.norm() < tolerance) {
             converged = true;
             break;
         }
 
         // Compute Jacobian
         Eigen::MatrixXd J = tree.compute_jacobian(joint_values, link_name);
 
         // Gradient descent step (pseudo-inverse of Jacobian)
         Eigen::VectorXd delta_theta = step_scale * J.completeOrthogonalDecomposition().solve(error);
 
         // Update joint values
         for (size_t j = 0; j < joint_values.size(); ++j) {
             joint_values[j] += delta_theta[j];
             current_js.joint_states[j].position = joint_values[j];
         }
     }
 
     return current_js;


    return {};
}

std::vector<rix::msg::sensor::JS> IKSolver::solve_with_trajectory(const std::string &link_name,
                                                                  const Eigen::Affine3d &goal,
                                                                  const rix::msg::sensor::JS &initial_guess,
                                                                  double step_scale, double tolerance,
                                                                  uint32_t max_iterations, bool &converged) {
    /**
     * TODO: Implement the inverse kinematics solver with trajectory. This 
     * function should return a vector of joint states that represent the
     * configuration of the robot at each step of the gradient descent
     * algorithm. The first element of the vector should be the initial guess, 
     * and the last element should be the final configuration that converged to 
     * the goal pose. The function should also set the converged flag to true
     * if the solution converged within the specified tolerance and maximum
     * iterations.
     */
    return {};
}

}  // namespace rdf
}  // namespace rix