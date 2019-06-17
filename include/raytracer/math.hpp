#ifndef RAYTRACER_MATH_HPP_INCLUDED
#define RAYTRACER_MATH_HPP_INCLUDED

#include <complex>

namespace rt
{

using complex = std::complex<float>;

float clamp(const float value, const float min, const float max);
bool  float_equal(const float lhs, const float rhs);
bool  float_not_equal(const float lhs, const float rhs);
float random(const float min = 0.0f, const float max = 1.0f);
} // namespace rt

#endif // RAYTRACER_MATH_HPP_INCLUDED
