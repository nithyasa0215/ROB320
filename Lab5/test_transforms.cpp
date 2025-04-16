#include <cmath>

#include <gtest/gtest.h>

#include "transforms.hpp"

static const double EPS = 1e-12;

Eigen::Vector3d applyTransform(const TransformMatrix& T, const Eigen::Vector3d& v) {
    Eigen::Vector4d hv(v.x(), v.y(), v.z(), 1.0);
    Eigen::Vector4d result = T * hv;
    return result.head<3>();
}

TEST(TransformTest, Rotation3DXAxis90Deg) {
    double angle = M_PI / 2.0;
    TransformMatrix M = rotation3DXAxis(angle);

    // Check matrix elements at the 3x3 rotation block below.
    // [ 1   0   0 ]
    // [ 0   0  -1 ]
    // [ 0   1   0 ]
    ASSERT_NEAR(M(0,0), 1.0, EPS);
    ASSERT_NEAR(M(1,1), 0.0, EPS);
    ASSERT_NEAR(M(1,2), -1.0, EPS);
    ASSERT_NEAR(M(2,1), 1.0, EPS);
    ASSERT_NEAR(M(2,2), 0.0, EPS);

    // Check a transformed vector.
    Eigen::Vector3d v(0, 1, 0);
    Eigen::Vector3d v_rot = applyTransform(M, v);
    ASSERT_NEAR(v_rot.x(), 0.0, EPS);
    ASSERT_NEAR(v_rot.y(), 0.0, EPS);
    ASSERT_NEAR(v_rot.z(), 1.0, EPS);
}

TEST(TransformTest, Rotation3DYAxis90Deg) {
    double angle = M_PI / 2.0;
    TransformMatrix M = rotation3DYAxis(angle);

    // Check matrix elements at the 3x3 rotation block:
    // [  0   0   1 ]
    // [  0   1   0 ]
    // [ -1   0   0 ]
    ASSERT_NEAR(M(0,0), 0.0, EPS);
    ASSERT_NEAR(M(1,1), 1.0, EPS);
    ASSERT_NEAR(M(2,2), 0.0, EPS);
    ASSERT_NEAR(M(0,2), 1.0, EPS);
    ASSERT_NEAR(M(2,0), -1.0, EPS);

    // Check a transformed vector:
    Eigen::Vector3d v(1, 0, 0);
    Eigen::Vector3d v_rot = applyTransform(M, v);
    ASSERT_NEAR(v_rot.x(), 0.0, EPS);
    ASSERT_NEAR(v_rot.y(), 0.0, EPS);
    ASSERT_NEAR(v_rot.z(), -1.0, EPS);
}

TEST(TransformTest, Rotation3DZAxis90Deg) {
    double angle = M_PI / 2.0;
    TransformMatrix M = rotation3DZAxis(angle);

    // Check matrix elements at the 3x3 rotation block:
    // [ 0  -1   0 ]
    // [ 1   0   0 ]
    // [ 0   0   1 ]
    ASSERT_NEAR(M(0,0), 0.0, EPS);
    ASSERT_NEAR(M(1,1), 0.0, EPS);
    ASSERT_NEAR(M(2, 2), 1.0, EPS);
    ASSERT_NEAR(M(0, 1), -1.0, EPS);
    ASSERT_NEAR(M(1, 0), 1.0, EPS);

    // Check a transformed vector:
    Eigen::Vector3d v(1, 0, 0);
    Eigen::Vector3d v_rot = applyTransform(M, v);
    ASSERT_NEAR(v_rot.x(), 0.0, EPS);
    ASSERT_NEAR(v_rot.y(), 1.0, EPS);
    ASSERT_NEAR(v_rot.z(), 0.0, EPS);
}

TEST(TransformTest, Rotation3D_XYZ) {
    // Rotate 90 about X, then Y, then Z.
    double rx = M_PI / 2.0;
    double ry = M_PI / 2.0;
    double rz = M_PI / 2.0;
    TransformMatrix M = rotation3D(rx, ry, rz, RotationConvention::XYZ);
    Eigen::Vector3d v(1, 0, 0);
    Eigen::Vector3d v_rot = applyTransform(M, v);
    ASSERT_NEAR(v_rot.x(), 0.0, EPS);
    ASSERT_NEAR(v_rot.y(), 0.0, EPS);
    ASSERT_NEAR(v_rot.z(), -1.0, EPS);
}

TEST(TransformTest, RotationInverse) {
    double angle = M_PI / 4.0;
    TransformMatrix R = rotation3DZAxis(angle);
    TransformMatrix R_inv = rotationInverse(R);

    // R * R_inv should be identity (within floating error).
    TransformMatrix I = R * R_inv;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            ASSERT_NEAR(I(i,j), expected, EPS);
        }
    }
}

TEST(TransformTest, Translation3D) {
    double tx = 1.0, ty = 2.0, tz = 3.0;
    TransformMatrix T = translation3D(tx, ty, tz);

    // Check a known vector (0,0,0) -> (1,2,3).
    Eigen::Vector3d v(0,0,0);
    Eigen::Vector3d v_trans = applyTransform(T, v);
    ASSERT_NEAR(v_trans.x(), 1.0, EPS);
    ASSERT_NEAR(v_trans.y(), 2.0, EPS);
    ASSERT_NEAR(v_trans.z(), 3.0, EPS);
}

TEST(TransformTest, TranslationInverse) {
    double tx = 1.0, ty = 2.0, tz = 3.0;
    TransformMatrix T = translation3D(tx, ty, tz);
    TransformMatrix T_inv = translationInverse(T);

    // T * T_inv should be the identity.
    TransformMatrix I = T * T_inv;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            ASSERT_NEAR(I(i,j), expected, EPS);
        }
    }
}

TEST(TransformTest, AffineTransform3D) {
    double rx = M_PI / 2.0, ry = 0.0, rz = 0.0;
    double tx = 1.0, ty = 2.0, tz = 3.0;
    TransformMatrix A = affineTransform3D(tx, ty, tz, rx, ry, rz, RotationConvention::XYZ);

    // Check a known vector.
    // If we rotate (0,1,0) about X by 90 -> (0,0,1), then translate -> (1,2,4).
    Eigen::Vector3d v(0,1,0);
    Eigen::Vector3d v_aff = applyTransform(A, v);
    ASSERT_NEAR(v_aff.x(), 1.0, EPS);
    ASSERT_NEAR(v_aff.y(), 2.0, EPS);
    ASSERT_NEAR(v_aff.z(), 4.0, EPS);
}

TEST(TransformTest, AffineInverse) {
    double rx = 0.0, ry = M_PI / 2.0, rz = 0.0;
    double tx = 1.0, ty = 2.0, tz = 3.0;
    TransformMatrix A = affineTransform3D(tx, ty, tz, rx, ry, rz, RotationConvention::XYZ);
    TransformMatrix A_inv = affineInverse(A);
    TransformMatrix I = A * A_inv;

    // Check that A * A_inv ~ Identity.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            ASSERT_NEAR(I(i,j), expected, EPS);
        }
    }
}

TEST(TransformTest, QuaternionFromAxisAngle) {
    Eigen::Vector3d x_axis(1, 0, 0);
    Eigen::Vector3d y_axis(0, 1, 0);
    Quaternion x_axis_90 = quaternionFromAxisAngle(x_axis, M_PI / 2);
    Quaternion y_axis_90 = quaternionFromAxisAngle(y_axis, M_PI / 2);
    Quaternion qA{0.7071, 0.7071, 0.0, 0.0};
    Quaternion qB{0.7071, 0.0, 0.7071, 0.0};
    ASSERT_NEAR(x_axis_90.w, qA.w, 1e-3);
    ASSERT_NEAR(x_axis_90.x, qA.x, 1e-3);
    ASSERT_NEAR(x_axis_90.y, qA.y, 1e-3);
    ASSERT_NEAR(x_axis_90.z, qA.z, 1e-3);
    ASSERT_NEAR(y_axis_90.w, qB.w, 1e-3);
    ASSERT_NEAR(y_axis_90.x, qB.x, 1e-3);
    ASSERT_NEAR(y_axis_90.y, qB.y, 1e-3);
    ASSERT_NEAR(y_axis_90.z, qB.z, 1e-3);
}

TEST(TransformTest, QuaternionMultiply) {
    // Simple check: multiply identity quaternion by another.
    Quaternion qId{1.0, 0.0, 0.0, 0.0};
    Quaternion qX{0.0, 1.0, 0.0, 0.0};
    Quaternion result = quaternionMultiply(qId, qX);
    ASSERT_NEAR(result.w, qX.w, EPS);
    ASSERT_NEAR(result.x, qX.x, EPS);
    ASSERT_NEAR(result.y, qX.y, EPS);
    ASSERT_NEAR(result.z, qX.z, EPS);

    // Check commutative property is not guaranteed.
    // Set 90-degree rotation about X.
    Quaternion qA{0.7071, 0.7071, 0.0, 0.0};

    // Set 90-degree rotation about Y.
    Quaternion qB{0.7071, 0.0, 0.7071, 0.0};
    Quaternion qAB = quaternionMultiply(qA, qB);
    Quaternion qBA = quaternionMultiply(qB, qA);

    // Confirm qAB != qBA.
    ASSERT_FALSE(std::fabs(qAB.w - qBA.w) < 1e-3 &&
                 std::fabs(qAB.x - qBA.x) < 1e-3 &&
                 std::fabs(qAB.y - qBA.y) < 1e-3 &&
                 std::fabs(qAB.z - qBA.z) < 1e-3);
}

TEST(TransformTest, QuaternionToRotation) {
    // A quaternion for a 90-degree rotation about the Z axis is:
    // q = [cos(45), 0, 0, sin(45)] = [0.7071, 0, 0, 0.7071]
    // The resulting 3x3 rotation:
    // [  0  -1   0 ]
    // [  1   0   0 ]
    // [  0   0   1 ]
    Quaternion q{0.7071, 0.0, 0.0, 0.7071};
    TransformMatrix R = quaternionToRotation(q);
    ASSERT_NEAR(R(0,0), 0.0,  1e-3);
    ASSERT_NEAR(R(0,1), -1.0, 1e-3);
    ASSERT_NEAR(R(1,0), 1.0,  1e-3);
    ASSERT_NEAR(R(1,1), 0.0,  1e-3);
    ASSERT_NEAR(R(2,2), 1.0, 1e-3);
}
