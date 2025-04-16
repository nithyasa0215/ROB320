#include <gtest/gtest.h>

#include "routines.hpp"

TEST(RoutineTest, CheckConventionsEquivalent) {
    // Check case where they are equivalent.
    ASSERT_TRUE(checkConventionsEquivalent(0, 0, M_PI,
        RotationConvention::XYZ,
        RotationConvention::ZYX));

    // Check case where they are not equivalent.
    double rx = M_PI / 4;
    double ry = M_PI / 4;
    double rz = M_PI / 5;
    ASSERT_FALSE(checkConventionsEquivalent(rx, ry, rz,
        RotationConvention::XYZ,
        RotationConvention::ZYX));
}

TEST(RoutineTest, CheckFrameAPointEquivalentEigen) {
    // Check case where they are equivalent (same transforms).
    TransformMatrix T_AB = TransformMatrix::Identity();
    Eigen::Vector3d axis_BC = Eigen::Vector3d::UnitZ();
    double angle_BC = M_PI / 4.0;
    Eigen::Affine3d T_eigen_AB = Eigen::Affine3d::Identity();
    Eigen::AngleAxisd angle_axis_eigen_BC(M_PI / 4.0, Eigen::Vector3d::UnitZ());
    Eigen::Vector3d point_C(1.0, 2.0, 3.0);
    ASSERT_TRUE(checkFrameAPointEquivalentEigen(
        T_AB,
        axis_BC,
        angle_BC,
        T_eigen_AB,
        angle_axis_eigen_BC,
        point_C
    ));

    // Check case where they are not equivalent (different transforms).
    TransformMatrix T_AB2 = TransformMatrix::Identity();
    Eigen::Vector3d axis_BC2 = Eigen::Vector3d::UnitZ();
    double angle_BC2 = M_PI / 4.0;
    Eigen::Affine3d T_eigen_AB2 = Eigen::Affine3d::Identity();

    // Rotate eigen frame by different amount.
    Eigen::AngleAxisd angle_axis_eigen_BC2(M_PI / 2.0, Eigen::Vector3d::UnitZ());
    Eigen::Vector3d point_C2(1.0, 2.0, 3.0);
    ASSERT_FALSE(checkFrameAPointEquivalentEigen(
        T_AB2,
        axis_BC2,
        angle_BC2,
        T_eigen_AB2,
        angle_axis_eigen_BC2,
        point_C2
    ));
}