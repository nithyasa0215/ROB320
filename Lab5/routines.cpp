#include "routines.hpp"

bool checkConventionsEquivalent(double rx, double ry, double rz,
                                RotationConvention convention1,
                                RotationConvention convention2) {
    // TODO: Create two rotation matrices from different conventions and return
    //       whether they are equivalent within a 1e-3 tolerance.
    TransformMatrix R1 = rotation3D(rx, ry, rz, convention1);
    TransformMatrix R2 = rotation3D(rx, ry, rz, convention2);
    TransformMatrix R1_inv = rotationInverse(R1);
    TransformMatrix R2_inv = rotationInverse(R2);
    TransformMatrix R1_R2 = R1_inv * R2;
    TransformMatrix R2_R1 = R2_inv * R1;
    TransformMatrix R1_R2_transpose = R1_R2.transpose();
    TransformMatrix R2_R1_transpose = R2_R1.transpose();
                                    
    if (R1_R2.isApprox(R1_R2_transpose, 1e-3) && R2_R1.isApprox(R2_R1_transpose, 1e-3)) {
        return true;
    }
    return false;
}

bool checkFrameAPointEquivalentEigen(const TransformMatrix& T_AB,
                                     const Eigen::Vector3d &axis_BC,
                                     double angle_BC,
                                     const Eigen::Affine3d &T_eigen_AB,
                                     const Eigen::AngleAxisd &angle_axis_eigen_BC,
                                     const Eigen::Vector3d &point_C) {
    // TODO: Transform point_C back to frame A once using locally defined
    //       transfrom functions and input and once using Eigen functions and
    //       input. Return whether the transformed points are equivalent within
    //       a 1e-3 tolerance.
    Eigen::Vector3d point_C_transformed = T_AB * point_C;
    Eigen::Vector3d point_C_transformed_eigen = T_eigen_AB * point_C;
    if (point_C_transformed.isApprox(point_C_transformed_eigen, 1e-3)) {
        return true;
    }
    // Do i need this line???
    Eigen::Vector3d point_C_transformed_back = T_eigen_AB.inverse() * point_C_transformed_eigen;
    return false;
}