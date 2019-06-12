#include <raytracer/math.hpp>
#include <algorithm>
#include <random>
#include "float.hpp"

namespace ray
{

float clamp(const float value, const float min, const float max)
{
    return std::clamp(value, min, max);
}

bool float_equal(const float lhs, const float rhs)
{
    return ray::detail::FloatingPoint<float>{lhs}.AlmostEquals(
        ray::detail::FloatingPoint<float>{rhs});
}

bool float_not_equal(const float lhs, const float rhs)
{
    return !ray::float_equal(lhs, rhs);
}

float random(const float min, const float max)
{
    static thread_local std::mt19937_64 prng{std::random_device{}()};
    std::uniform_real_distribution<float> dist{min, max};

    return dist(prng);
}

}
