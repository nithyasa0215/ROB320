#pragma once

#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stack>
#include <string>
#include <vector>

#include "rix/util/timing.hpp"
#include "rix/msg/geometry/TF.hpp"
#include "rix/msg/geometry/TransformStamped.hpp"
#include "rix/rdf/joint.hpp"
#include "rix/rdf/link.hpp"
#include "rix/rdf/tree.hpp"
#include "rix/rdf/util.hpp"

namespace rix {
namespace rdf {

class FKSolver {
   public:
    FKSolver(const Tree &tree);
    Eigen::Affine3d solve(const std::string &link_name) const;
    Eigen::Affine3d solve(const std::string &link_name, const std::string &reference_link) const;
    static Eigen::Affine3d solve(const rix::msg::geometry::TF &tf, const std::string &link_name);
    static Eigen::Affine3d solve(const rix::msg::geometry::TF &tf, const std::string &link_name,
                                 const std::string &reference_link);
    rix::msg::geometry::TF global_tf() const;
    static rix::msg::geometry::TF global_tf(const rix::msg::geometry::TF &tf);

   private:
    const Tree &tree;

    void solve_recursive_helper(const std::string &link_name, Eigen::Affine3d &transform) const;

    static void solve_recursive_helper(const std::unordered_map<std::string, Eigen::Affine3d> &transform_map,
                                       const std::unordered_map<std::string, std::string> &parent_map,
                                       Eigen::Affine3d &transform, const std::string &link);

    void global_tf_recursive_helper(const std::string &link_name, rix::msg::geometry::TF &global_tf,
                                    std::stack<Eigen::Affine3d> &transform_stack, size_t &index) const;

    static void global_tf_recursive_helper(const std::unordered_map<std::string, Eigen::Affine3d> &transform_map,
                                           const std::unordered_map<std::string, std::vector<std::string>> &child_map,
                                           const Eigen::Affine3d &transform, const std::string &link_name,
                                           rix::msg::geometry::TF &tf, size_t &index);
};

}  // namespace rdf
}  // namespace rix