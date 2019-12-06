#include "float.hpp"
#include <algorithm>
#include <random>
#include <raytracer/debug/profile.hpp>
#include <raytracer/math.hpp>

namespace rt
{

float clamp(const float value, const float min, const float max)
{
    return std::clamp(value, min, max);
}

bool float_equal(const float lhs, const float rhs)
{
    return rt::detail::FloatingPoint<float>{lhs}.AlmostEquals(
        rt::detail::FloatingPoint<float>{rhs});
}

bool float_not_equal(const float lhs, const float rhs)
{
    return !rt::float_equal(lhs, rhs);
}

float random(const float min, const float max)
{
    RT_PROFILE_FUNCTION();

    static thread_local std::mt19937_64   prng{std::random_device{}()};
    std::uniform_real_distribution<float> dist{min, max};

    return dist(prng);
}

} // namespace rt
