#include "robot_parser.hpp"

int main () {
    KinematicTree tree("my_robot.json");

    assert(tree.root == "base");
    assert(tree.joints.size() == 3);
    assert(tree.links.size() == 4);

    assert(tree.joints["shoulder"].parent == "base");
    assert(tree.joints["shoulder"].child == "upper_arm");
    assert(tree.joints["shoulder"].axis.isApprox(Eigen::Vector3d(0, 1, 0)));
    assert(tree.joints["shoulder"].origin.matrix().isApprox(Eigen::Affine3d::Identity().matrix()));
    assert(tree.joints["shoulder"].type == REVOLUTE);

    assert(tree.joints["elbow"].parent == "upper_arm");
    assert(tree.joints["elbow"].child == "forearm");
    assert(tree.joints["elbow"].axis.isApprox(Eigen::Vector3d(0, 1, 0)));
    assert(tree.joints["elbow"].origin.translation().isApprox(Eigen::Vector3d(0.5, 0, 0)));
    assert(tree.joints["elbow"].origin.rotation().eulerAngles(0, 1, 2).isApprox(Eigen::Vector3d(0, 0, 0)));
    assert(tree.joints["elbow"].type == REVOLUTE);

    assert(tree.joints["wrist"].parent == "forearm");
    assert(tree.joints["wrist"].child == "hand");
    assert(tree.joints["wrist"].axis.isApprox(Eigen::Vector3d(1, 0, 0)));
    assert(tree.joints["wrist"].origin.translation().isApprox(Eigen::Vector3d(0.5, 0, 0)));
    assert(tree.joints["wrist"].origin.rotation().eulerAngles(0, 1, 2).isApprox(Eigen::Vector3d(0, 0, 0)));
    assert(tree.joints["wrist"].type == PRISMATIC);

    assert(tree.links["base"].origin.matrix().isApprox(Eigen::Affine3d::Identity().matrix()));
    assert(tree.links["base"].parent == "");
    assert(tree.links["base"].children.size() == 1);

    assert(tree.links["upper_arm"].origin.translation().isApprox(Eigen::Vector3d(0.5, 0, 0)));
    assert(tree.links["upper_arm"].origin.rotation().eulerAngles(0, 1, 2).isApprox(Eigen::Vector3d(0, 0.785398, 0)));
    assert(tree.links["upper_arm"].parent == "shoulder");
    assert(tree.links["upper_arm"].children.size() == 1);

    assert(tree.links["forearm"].origin.translation().isApprox(Eigen::Vector3d(0.5, 0, 0)));
    assert(tree.links["forearm"].origin.rotation().eulerAngles(0, 1, 2).isApprox(Eigen::Vector3d(0, -0.785398, 0)));
    assert(tree.links["forearm"].parent == "elbow");
    assert(tree.links["forearm"].children.size() == 1);

    assert(tree.links["hand"].origin.matrix().isApprox(Eigen::Affine3d::Identity().matrix()));
    assert(tree.links["hand"].parent == "wrist");
    assert(tree.links["hand"].children.size() == 0);
}