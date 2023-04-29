#pragma once

#ifdef __ANDROID__
#include <type_traits>
#endif

#include <Eigen/Core>

namespace juglans {

namespace internal {

#ifdef __ANDROID__
// NOTE: on android r21e, template specialization causes multiple definition
template <typename T> constexpr T epsilon = std::is_same_v<T, float> ? T(1e-6f) : T(1e-10);
#else
template <typename T> constexpr T epsilon = T(1e-10);
template <> constexpr float epsilon<float> = 1e-6f;
#endif

template <typename T> constexpr T PI = T(3.1415926535897932385L);

template <typename T> constexpr T Degree2Radian = PI<T> / T(180);
}  // namespace internal

using number_t = float;
constexpr number_t epsilon = internal::epsilon<number_t>;
constexpr number_t PI = internal::PI<number_t>;
constexpr number_t Degree2Radian = internal::Degree2Radian<number_t>;

inline constexpr number_t Radian(number_t degree) { return degree * Degree2Radian; }

using point2_t = Eigen::Matrix<number_t, 2, 1>;
using point3_t = Eigen::Matrix<number_t, 3, 1>;
using matrix22_t = Eigen::Matrix<number_t, 2, 2>;
using matrix13_t = Eigen::Matrix<number_t, 1, 3>;
using matrix23_t = Eigen::Matrix<number_t, 2, 3>;
using matrix33_t = Eigen::Matrix<number_t, 3, 3>;

using vec_point3_t = std::vector<point3_t, Eigen::aligned_allocator<point3_t>>;
using vec_vec_point3_t = std::vector<vec_point3_t, Eigen::aligned_allocator<vec_point3_t>>;

}  // namespace juglans
