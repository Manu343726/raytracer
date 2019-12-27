#include <raytracer/debug/profile.hpp>
#include <raytracer/ray.hpp>

using namespace rt;

ray::ray(const vector& origin, const vector& direction)
    : _origin{origin}, _direction{direction}
{
}

const vector& ray::origin() const
{
    return _origin;
}

const vector& ray::direction() const
{
    return _direction;
}

vector ray::point(const float t) const
{
    ZoneScoped;
    return _origin + _direction * t;
}

ray ray::from_to(const vector& from, const vector& to)
{
    ZoneScoped;
    return {from, (to - from).normalized()};
}
