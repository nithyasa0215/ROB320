#include "rix/rdf/util.hpp"

namespace rix {
namespace rdf {

rix::msg::geometry::Transform eigen_to_msg(const Eigen::Affine3d &T) {
    rix::msg::geometry::Transform t;
    Eigen::Vector3d translation = T.translation();
    t.translation.x = translation.x();
    t.translation.y = translation.y();
    t.translation.z = translation.z();

    Eigen::Quaterniond q(T.rotation());
    t.rotation.w = q.w();
    t.rotation.x = q.x();
    t.rotation.y = q.y();
    t.rotation.z = q.z();
    return t;
}

Eigen::Affine3d msg_to_eigen(const rix::msg::geometry::Transform &t) {
    Eigen::Affine3d T = Eigen::Affine3d::Identity();
    Eigen::Vector3d translation(t.translation.x, t.translation.y, t.translation.z);
    T.translation() = translation;

    Eigen::Quaterniond q(t.rotation.w, t.rotation.x, t.rotation.y, t.rotation.z);
    T.linear() = q.toRotationMatrix();
    return T;
}


}
}