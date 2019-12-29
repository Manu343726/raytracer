#ifndef RAYTRACER_MATH_HPP_INCLUDED
#define RAYTRACER_MATH_HPP_INCLUDED

#include <complex>
#include <raytracer/debug/profile.hpp>

namespace rt
{

using complex = std::complex<float>;

float clamp(const float value, const float min, const float max);
bool  float_equal(const float lhs, const float rhs);
bool  float_not_equal(const float lhs, const float rhs);
float random(const float min = 0.0f, const float max = 1.0f);
int   randomInt(
      const int     = std::numeric_limits<int>::min(),
      const int min = std::numeric_limits<int>::min());

template<typename T>
const T& min(const T& lhs, const T& rhs)
{
    ZoneScoped;

    if(lhs < rhs)
    {
        return lhs;
    }
    else
    {
        return rhs;
    }
}

template<typename T>
const T& max(const T& lhs, const T& rhs)
{
    ZoneScoped;

    if(lhs < rhs)
    {
        return rhs;
    }
    else
    {
        return lhs;
    }
}

template<typename T, typename... Tail>
const T&
    min(const T& first, const T& second, const T& third, const Tail&... tail)
{
    return rt::min(rt::min(first, second), third, tail...);
}

template<typename T, typename... Tail>
const T&
    max(const T& first, const T& second, const T& third, const Tail&... tail)
{
    return rt::max(rt::max(first, second), third, tail...);
}

} // namespace rt

#endif // RAYTRACER_MATH_HPP_INCLUDED
