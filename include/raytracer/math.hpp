#ifndef RAYTRACER_MATH_HPP_INCLUDED
#define RAYTRACER_MATH_HPP_INCLUDED

namespace rt
{

float clamp(const float value, const float min, const float max);
bool float_equal(const float lhs, const float rhs);
bool float_not_equal(const float lhs, const float rhs);
float random(const float min = 0.0f, const float max = 1.0f);

}

#endif // RAYTRACER_MATH_HPP_INCLUDED
