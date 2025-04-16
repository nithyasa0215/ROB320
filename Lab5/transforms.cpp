#include "transforms.hpp"

TransformMatrix rotation3DXAxis(double angle) {
    // TODO: Create a rotation matrix for rotating around the X axis.
    Eigen::Matrix4d mat4d;
    mat4d << 1, 0, 0, 0,
            0, cos(angle), -sin(angle),0,
            0, sin(angle), cos(angle),0,
            0,0,0,1;
    return mat4d;
}

TransformMatrix rotation3DYAxis(double angle) {
    // TODO: Create a rotation matrix for rotating around the Y axis.
    Eigen::Matrix4d mat4d;
    mat4d << cos(angle),0,sin(angle), 0,
            0, 1, 0,0,
            -sin(angle), 0, cos(angle),0,
            0,0,0,1;
    return mat4d;
}

TransformMatrix rotation3DZAxis(double angle) {
    // TODO: Create a rotation matrix for rotating around the Z axis.
    Eigen::Matrix4d mat4d;
    mat4d << cos(angle), -sin(angle), 0, 0,
            sin(angle), cos(angle), 0,0,
            0, 0, 1,0,
            0,0,0,1;
    return mat4d;
}

TransformMatrix rotation3D(double rx, double ry, double rz,
                           RotationConvention convention) {
    // TODO: Create a rotation matrix for a full rotation via the given
    //       convention. (Order represents multiplication right to left.)
    TransformMatrix Rx = rotation3DXAxis(rx);
    TransformMatrix Ry = rotation3DYAxis(ry);
    TransformMatrix Rz = rotation3DZAxis(rz);
    switch(convention){
        case RotationConvention :: XYZ:
            return Rz * Ry * Rx;
        case RotationConvention :: XZY:
            return Ry * Rz * Rx;
        case RotationConvention:: YXZ:
            return Rz * Rx * Ry;
        case RotationConvention:: YZX:
            return Rx * Rz * Ry;
        case RotationConvention::ZXY:
            return Ry * Rx * Rz; 
        case RotationConvention::ZYX:
            return Rx * Ry * Rz; 
    }
    return TransformMatrix::Identity();
}

TransformMatrix rotationInverse(const TransformMatrix& rotation_matrix) {
    // TODO: Return the inverse of the given matrix, assuming the input is a valid
    //       rotation matrix. You should not need to used matrix inversion.
    // For an orthonormal rotation matrix, the inverse is the transpose.
    return rotation_matrix.transpose();
}

TransformMatrix translation3D(double tx, double ty, double tz) {
    // TODO: Create a matrix that applies the specified translation.
    TransformMatrix T = TransformMatrix::Identity();
    T(0,3) = tx;
    T(1,3) = ty;
    T(2,3) = tz;
    return T;
}

TransformMatrix translationInverse(const TransformMatrix& translation_matrix) {
    // TODO: Return the inverse of the given matrix, assuming it is a valid
    //       translation matrix. You should not need to use matrix inversion.
    TransformMatrix T = TransformMatrix::Identity();
    T(0,3) = -translation_matrix(0,3);
    T(1,3) = -translation_matrix(1,3);
    T(2,3) = - translation_matrix(2,3);

    return T;
}

TransformMatrix affineTransform3D(double tx, double ty, double tz,
                                  double rx, double ry, double rz,
                                  RotationConvention convention) {
    // TODO: Create a matrix that applies both a translation and a rotation.
    //       Rotate then translate!!!
    //TransformMatrix T = TransformMatrix::Identity();
    TransformMatrix rotation = rotation3D(rx,ry,rz,convention);
    TransformMatrix translation = translation3D(tx,ty,tz);
    TransformMatrix affine = translation * rotation; 
    return affine;
}

TransformMatrix affineInverse(const TransformMatrix& affine_matrix) {
    // TODO: Return the inverse of the given matrix, assuming it is a valid
    //       affine transformation matrix. You should not need to use matrix
    //       inversion.
    Eigen::Matrix3d rotation = affine_matrix.block<3,3>(0,0);
    Eigen::Vector3d translation = affine_matrix.block<3,1>(0,3);

    Eigen::Matrix3d rot_inv = rotation.transpose();
    Eigen::Vector3d trans_inv = -rot_inv * translation;

    TransformMatrix affineInverse = TransformMatrix::Identity();
    affineInverse.block<3,3>(0,0) = rot_inv;
    affineInverse.block<3,1>(0,3) = trans_inv;
    return affineInverse;
}

Quaternion quaternionFromAxisAngle(const Eigen::Vector3d& axis, double angle) {
    // TODO: Create a quaternion representing this axis-angle rotation.
    Eigen::Vector3d vector = axis.normalized();
    double halfAngle = angle/2;

    Quaternion q; 
    //q.w = halfAngle;
    q.w = cos(halfAngle);
    q.x = vector.x() * sin(halfAngle);
    q.y = vector.y() * sin(halfAngle);
    q.z = vector.z() * sin(halfAngle);
    return q;
}

Quaternion quaternionMultiply(const Quaternion& q1, const Quaternion& q2) {
    // TODO: Return the product of these two quaternions.
    Quaternion product;

    // equation on slides
    product.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z; 
    product.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y; 
    product.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x; 
    product.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w; 

    return product;
}

TransformMatrix quaternionToRotation(const Quaternion& q) {
    // TODO: Convert the given quaternion to a 4x4 rotation matrix.

    Eigen::Matrix3d mat3d;
    mat3d << ((q.w * q.w) + (q.x * q.x)-(q.y*q.y)-(q.x*q.z)), (2*((q.x*q.y)-(q.w*q.z))), (2*((q.w*q.y)-(q.x*q.z))),
            (2*((q.x*q.y)+(q.w*q.z))), ((q.w*q.w)-(q.x*q.x)+(q.y*q.y)-(q.z*q.z)), (2*((q.y*q.x)-(q.w*q.x))),
            (2*((q.x*q.z)-(q.w*q.y))), (2*(q.w*q.x)+(q.y*q.z)), ((q.w*q.w)-(q.x*q.x)-(q.y*q.y)+(q.z*q.z));

    TransformMatrix mat4d = TransformMatrix::Identity();
    mat4d.block<3,3>(0,0) = mat3d;            
    return mat4d;
}
