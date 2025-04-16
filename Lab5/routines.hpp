#ifndef ROUTINES_HPP
#define ROUTINES_HPP

#include "transforms.hpp"

/**
 * @brief Generate rotation transforms with two different conventions. Returns
 * true if they're equivalent and false otherwise.
 *
 * NOTE: Use a tolerance of 1e-3 for comparing entries.
 */
bool checkConventionsEquivalent(double rx, double ry, double rz,
                                RotationConvention convention1,
                                RotationConvention convention2);

/**
 * @brief Transform a point in frame C back to frame A once using locally
 * defined transform functions, and once using Eigen transform functions.
 * Return true if the transformed points are equivalent and false otherwise.
 *
 * NOTE: Use a tolerance of 1e-3 for comparing entries.
 */
bool checkFrameAPointEquivalentEigen(const TransformMatrix& T_AB,
                                     const Eigen::Vector3d &axis_BC,
                                     double angle_BC,
                                     const Eigen::Affine3d &T_eigen_AB,
                                     const Eigen::AngleAxisd &angle_axis_eigen_BC,
                                     const Eigen::Vector3d &point_C);

#endif //ROUTINES_HPP
