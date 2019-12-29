#include <raytracer/box.hpp>

#include <raytracer/debug/profile.hpp>
#include <raytracer/math.hpp>
#include <raytracer/segment.hpp>

using namespace rt;

box::box(const rt::vector& min, const rt::vector& max) : _min{min}, _max{max} {}

box box::centered(const rt::vector& center, const float radious)
{
    ZoneScoped;
    return centered(center, {radious, radious, radious});
}

box box::centered(const rt::vector& center, const rt::vector& radious)
{
    return {center - radious, center + radious};
}

box box::infinite()
{
    return {{std::numeric_limits<float>::min(),
             std::numeric_limits<float>::min(),
             std::numeric_limits<float>::min()},
            {std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max()}};
}

box box::nil()
{
    return {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
}

bool box::contains(const rt::vector& point) const
{
    return _min <= point && point <= _max;
}

bool box::contains(const rt::ray& ray) const
{
    return contains(ray, rt::ray::MIN_T, rt::ray::MAX_T);
}

bool box::contains(
    const rt::ray& ray, const float min_t, const float max_t) const
{
    ZoneScoped;

    for(int axis = 0; axis < 3; ++axis)
    {
        ZoneNamedN(AxisZone, "Axis test", true);

        const float t0 = rt::min(ray.t(_min, axis), ray.t(_max, axis));
        const float t1 = rt::max(ray.t(_min, axis), ray.t(_max, axis));

        const float tmin = rt::max(min_t, t0);
        const float tmax = rt::min(max_t, t1);

        if(tmax <= tmin)
        {
            return false;
        }
    }

    return true;
}

const vector& box::min() const
{
    return _min;
}

const vector& box::max() const
{
    return _max;
}

vector box::center() const
{
    return min() + size() * 0.5f;
}

vector box::size() const
{
    return _max - _min;
}

namespace rt
{

box surrounding_box(const box& lhs, const box& rhs)
{
    return {rt::min(lhs.min(), rhs.min()), rt::max(lhs.max(), rhs.max())};
}

} // namespace rt
