#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include <Eigen/Dense>

/**
 * @brief Use eigen 4x4 matrix of doubles with TransformMatrix.
 */
using TransformMatrix = Eigen::Matrix4d;

/**
 * @brief Enumerate possible Euler angle conventions for composing rotations.
 * For example, XYZ means: (1) rotate about X, then (2) rotate about Y, then
 * (3) rotate about Z. Because matrix multiplication is right to left when
 * transforming a vector, the resulting matrix is Rz * Ry * Rx.
 */
enum class RotationConvention {
    XYZ,
    XZY,
    YXZ,
    YZX,
    ZXY,
    ZYX
};

/**
 * @brief A simple struct for quaternions.
 */
struct Quaternion {
    double w;
    double x;
    double y;
    double z;
};

/**
 * @brief Returns a 4x4 rotation matrix representing a rotation about the X-axis.
 */
TransformMatrix rotation3DXAxis(double angle);

/**
 * @brief Returns a 4x4 rotation matrix representing a rotation about the Y-axis.
 */
TransformMatrix rotation3DYAxis(double angle);

/**
 * @brief Return a 4x4 rotation matrix representing a rotation about the Z-axis.
 */
TransformMatrix rotation3DZAxis(double angle);

/**
 * @brief Return a 4x4 rotation matrix by composing rotations about X, Y, and Z
 * in a specified convention (e.g., XYZ, ZYX, etc.).
 */
TransformMatrix rotation3D(double rx, double ry, double rz,
                           RotationConvention convention);

/**
 * @brief Assumes the given 4x4 matrix is a pure rotation matrix (orthonormal),
 * and returns its transpose.
 */
TransformMatrix rotationInverse(const TransformMatrix& rotation_matrix);

/**
 * @brief Returns a 4x4 matrix representing a translation by (tx, ty, tz).
 */
TransformMatrix translation3D(double tx, double ty, double tz);

/**
 * @brief Assumes the given 4x4 matrix is a pure translation matrix,
 * and returns the 4x4 matrix that translates in the opposite direction.
 */
TransformMatrix translationInverse(const TransformMatrix& translation_matrix);

/**
 * @brief Returns a 4x4 transform representing rotation (rx, ry, rz, with a given
 * convention) and translation (tx, ty, tz). The final transformation is T * R,
 * meaning we rotate first, then translate the point.
 */
TransformMatrix affineTransform3D(double tx, double ty, double tz,
                                  double rx, double ry, double rz,
                                  RotationConvention convention);
/**
 * @brief Assumes the given 4x4 matrix is an affine transform consisting of a
 * pure rotation (orthonormal 3x3 block) and a translation in the last column.
 * Returns the inverse of that affine transform.
 */
TransformMatrix affineInverse(const TransformMatrix& affine_matrix);

/**
 * @brief Generates a quaternion from an axis and an angle of rotation about
 * that axis.
 */
Quaternion quaternionFromAxisAngle(const Eigen::Vector3d& axis, double angle);

/**
 * @brief Multiplies two quaternions q1 * q2 and returns the result.
 */
Quaternion quaternionMultiply(const Quaternion& q1, const Quaternion& q2);

/**
 * @brief Converts a quaternion to a 4x4 rotation matrix (the 3x3 block is
 * the standard quaternion-to-rotation conversion, and the last row/column
 * are [0 0 0 1]).
 */
TransformMatrix quaternionToRotation(const Quaternion& q);

#endif //TRANSFORMS_HPP
