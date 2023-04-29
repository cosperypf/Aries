#pragma once

#include <cmath>

#include <Eigen/Geometry>

#include "juglans/core/types.hpp"

namespace juglans {

using Vector3 = point3_t;
using Quaternion = Eigen::Quaternion<number_t>;

inline Vector3 operator*(const Quaternion &q, const Vector3 &v) {
  Vector3 u = q.vec();
  return 2.0f * u.dot(v) * u + (q.w() * q.w() - u.dot(u)) * v + 2.0f * q.w() * u.cross(v);
}

#pragma pack(push, 4)

// NOTE: following structs will be used in Unity3D, must use POD types

struct Transform {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  Transform() {
    position << 0.f, 0.f, 0.f;
    rotation = Quaternion::Identity();
  }

  Transform(const Vector3 &pos, const Quaternion &rot) : position(pos), rotation(rot) {}

  Transform &operator*=(const Transform &other) {
    position += rotation * other.position;
    rotation *= other.rotation;
    return *this;
  }

  friend Transform operator*(Transform left, const Transform &right) {
    left *= right;
    return left;
  }

  Transform &operator*=(const Quaternion &q) {
    rotation *= q;
    return *this;
  }

  friend Transform operator*(Transform t, const Quaternion &q) {
    t *= q;
    return t;
  }

  friend Transform operator*(const Quaternion &q, Transform t) {
    t.rotation = q * t.rotation;
    t.position = q * t.position;
    return t;
  }

  friend Vector3 operator*(const Transform &t, const Vector3 &v) {
    return t.rotation * v + t.position;
  }

  Transform inverse() const {
    Transform t(-position, rotation.inverse());
    t.position = t.rotation * t.position;
    return t;
  }

  Vector3 position;
  Quaternion rotation;
};

#pragma pack(pop)

}  // namespace juglans
