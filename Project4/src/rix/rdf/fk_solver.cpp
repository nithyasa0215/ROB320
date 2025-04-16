#include <rix/rdf/fk_solver.hpp>

namespace rix {
namespace rdf {

FKSolver::FKSolver(const Tree &tree) : tree(tree) {}

Eigen::Affine3d FKSolver::solve(const std::string &link_name) const {
    /**
     * TODO: Return the global transformation of the link.
     */

    return Eigen::Affine3d::Identity();
}

Eigen::Affine3d FKSolver::solve(const std::string &link_name, const std::string &reference_link) const {
    /**
     * TODO: Return the transformation of the link relative to the reference link.
     */
    return Eigen::Affine3d::Identity();
}

Eigen::Affine3d FKSolver::solve(const rix::msg::geometry::TF &tf, const std::string &link_name) {
    /**
     * TODO: Given a TF message, return the global transformation of the link.
     */
    return Eigen::Affine3d::Identity();
}

Eigen::Affine3d FKSolver::solve(const rix::msg::geometry::TF &tf, const std::string &link_name,
                                const std::string &reference_link) {
    /**
     * TODO: Given a TF message, return the transformation of the link relative to the reference link.
     */
    return Eigen::Affine3d::Identity();
}

rix::msg::geometry::TF FKSolver::global_tf() const {
    /**
     * TODO: Generate a global TF message from the tree. This message should
     * contain the global transformation of each link in the tree.
     */
    return rix::msg::geometry::TF();
}

rix::msg::geometry::TF FKSolver::global_tf(const rix::msg::geometry::TF &tf) {
    /**
     * TODO: Given a TF message, return a new TF message containing the global
     * transformation of each link in the input message.
     */
    return rix::msg::geometry::TF();
}

}  // namespace rdf
}  // namespace rix