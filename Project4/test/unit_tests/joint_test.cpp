#include "rix/rdf/joint.hpp"

#include <gtest/gtest.h>

#include <Eigen/Geometry>

using namespace rix::rdf;

#define TOL 0.00001

TEST(JointTest, DefaultConstructor) {
    Joint joint;
    EXPECT_EQ(joint.get_type(), FIXED);
    EXPECT_EQ(joint.get_parent(), "");
    EXPECT_EQ(joint.get_child(), "");
    EXPECT_EQ(joint.get_lower_bound(), 0.0);
    EXPECT_EQ(joint.get_upper_bound(), 0.0);
    EXPECT_TRUE(joint.get_axis().isApprox(Eigen::Vector3d::Zero(), TOL));
    EXPECT_TRUE(joint.get_origin().isApprox(Eigen::Affine3d::Identity(), TOL));
}

TEST(JointTest, ParameterizedConstructor) {
    Eigen::Vector3d axis(1, 0, 0);
    Eigen::Affine3d origin(Eigen::Translation3d(1, 2, 3));
    Joint joint(axis, origin, REVOLUTE, "parent_link", "child_link", 1.0, -1.0);

    EXPECT_EQ(joint.get_type(), REVOLUTE);
    EXPECT_EQ(joint.get_parent(), "parent_link");
    EXPECT_EQ(joint.get_child(), "child_link");
    EXPECT_EQ(joint.get_lower_bound(), -1.0);
    EXPECT_EQ(joint.get_upper_bound(), 1.0);
    EXPECT_TRUE(joint.get_axis().isApprox(axis, TOL));
    EXPECT_TRUE(joint.get_origin().translation().isApprox(origin.translation(), TOL));
}

TEST(JointTest, CopyConstructor) {
    Joint joint1(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), REVOLUTE, "parent_link",
                 "child_link", 1.0, -1.0);
    Joint joint2(joint1);

    EXPECT_EQ(joint2.get_type(), joint1.get_type());
    EXPECT_EQ(joint2.get_parent(), joint1.get_parent());
    EXPECT_EQ(joint2.get_child(), joint1.get_child());
    EXPECT_EQ(joint2.get_lower_bound(), joint1.get_lower_bound());
    EXPECT_EQ(joint2.get_upper_bound(), joint1.get_upper_bound());
    EXPECT_TRUE(joint2.get_axis().isApprox(joint1.get_axis(), TOL));
    EXPECT_TRUE(joint2.get_origin().translation().isApprox(joint1.get_origin().translation(), TOL));
}

TEST(JointTest, AssignmentOperator) {
    Joint joint1(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), REVOLUTE, "parent_link",
                 "child_link", 1.0, -1.0);
    Joint joint2;
    joint2 = joint1;

    EXPECT_EQ(joint2.get_type(), joint1.get_type());
    EXPECT_EQ(joint2.get_parent(), joint1.get_parent());
    EXPECT_EQ(joint2.get_child(), joint1.get_child());
    EXPECT_EQ(joint2.get_lower_bound(), joint1.get_lower_bound());
    EXPECT_EQ(joint2.get_upper_bound(), joint1.get_upper_bound());
    EXPECT_TRUE(joint2.get_axis().isApprox(joint1.get_axis(), TOL));
    EXPECT_TRUE(joint2.get_origin().translation().isApprox(joint1.get_origin().translation(), TOL));
}

TEST(JointTest, IsInBounds) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), REVOLUTE, "parent_link",
                "child_link", 1.0, -1.0);
    EXPECT_TRUE(joint.is_in_bounds(0.0));
    EXPECT_TRUE(joint.is_in_bounds(1.0));
    EXPECT_TRUE(joint.is_in_bounds(-1.0));
    EXPECT_FALSE(joint.is_in_bounds(1.1));
    EXPECT_FALSE(joint.is_in_bounds(-1.1));
}

TEST(JointTest, Clamp) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), REVOLUTE, "parent_link",
                "child_link", 1.0, -1.0);
    EXPECT_EQ(joint.clamp(0.0), 0.0);
    EXPECT_EQ(joint.clamp(1.0), 1.0);
    EXPECT_EQ(joint.clamp(-1.0), -1.0);
    EXPECT_EQ(joint.clamp(1.1), 1.0);
    EXPECT_EQ(joint.clamp(-1.1), -1.0);
}

TEST(JointTest, SetState) {
    Joint joint;
    joint.set_state(0.5);
    EXPECT_EQ(joint.get_state(), 0.5);
    joint.set_state(1.5);
    EXPECT_EQ(joint.get_state(), 1.5);
}

TEST(JointTest, GetTransformFixed) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), FIXED, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(0.5);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 1, 
                         0, 1, 0, 2, 
                         0, 0, 1, 3, 
                         0, 0, 0, 1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformRevolute) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), REVOLUTE, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(0.5);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();

    Eigen::Affine3d expected;
    expected.matrix() << 1,        0,         0, 1, 
                         0, 0.877583, -0.479426, 2, 
                         0, 0.479426,  0.877583, 3, 
                         0,        0,         0, 1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformPrismatic) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), PRISMATIC, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(0.5);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 1.5, 
                         0, 1, 0,   2, 
                         0, 0, 1,   3, 
                         0, 0, 0,   1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformContinuous) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), CONTINUOUS, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(0.5 + 4 * M_PI);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1,        0,         0, 1, 
                         0, 0.877583, -0.479426, 2, 
                         0, 0.479426,  0.877583, 3, 
                         0,        0,         0, 1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformFixed2) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), FIXED, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(-1.0);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 1, 
                         0, 1, 0, 2, 
                         0, 0, 1, 3, 
                         0, 0, 0, 1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformRevolute2) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), REVOLUTE, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(-1.0);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1,         0,        0, 1, 
                         0,  0.540302, 0.841471, 2, 
                         0, -0.841471, 0.540302, 3, 
                         0,         0,        0, 1;
    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformPrismatic2) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), PRISMATIC, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(-1.0);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 0, 
                         0, 1, 0, 2, 
                         0, 0, 1, 3, 
                         0, 0, 0, 1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

TEST(JointTest, GetTransformContinuous2) {
    Joint joint(Eigen::Vector3d(1, 0, 0), Eigen::Affine3d(Eigen::Translation3d(1, 2, 3)), CONTINUOUS, "parent_link",
                "child_link", 1.0, -1.0);
    joint.set_state(-1.0 - 4 * M_PI);
    Eigen::Affine3d transform = joint.get_origin() * joint.get_transform();
    std::cout << transform.matrix() << std::endl;

    Eigen::Affine3d expected;
    expected.matrix() << 1,         0,         0, 1, 
                         0,  0.540302,  0.841471, 2, 
                         0, -0.841471,  0.540302, 3, 
                         0,         0,         0, 1;

    EXPECT_TRUE(transform.matrix().isApprox(expected.matrix(), TOL));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}