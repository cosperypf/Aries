#pragma once

#include <opencv2/core.hpp>

#include "juglans/core/transform.hpp"

namespace juglans {

constexpr size_t HAND_MAX_POINTS = 21;
constexpr size_t HAND_MAX_ANGLES = 20;

enum class Handness { Left = 0, Right = 1 };

struct Hand2D {
  int id;
  size_t batch;
  cv::Rect box;
  float prob;
  Handness handness;

  cv::Point2f keypoint[HAND_MAX_POINTS];
  bool hasDepth;
  float depth[HAND_MAX_POINTS];
};

#pragma pack(push, 4)

// NOTE: following structs will be used in Unity3D, must use POD types

struct Hand3D {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  // make eigen types in the front
  Quaternion rotation;
  Vector3 keypoints[HAND_MAX_POINTS];
  // wrist + angles
  number_t error[HAND_MAX_ANGLES + 1];
  number_t angles[HAND_MAX_ANGLES];

  int id;
  float prob;
  bool hasPoints;
  Handness handness;
};

#pragma pack(pop)

}  // namespace juglans
