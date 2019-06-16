#include <raytracer/ray.hpp>

using namespace rt;

ray::ray(const vector& origin, const vector& direction) :
    _origin{origin},
    _direction{direction}
{}

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
    return _origin + _direction * t;
}
