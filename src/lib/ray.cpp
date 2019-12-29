#include <limits>
#include <raytracer/debug/profile.hpp>
#include <raytracer/math.hpp>
#include <raytracer/ray.hpp>

using namespace rt;

const float ray::MIN_T;
const float ray::MAX_T;

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

float t(const float a, const float b, const float x)
{
    ZoneScoped;

    if(rt::float_equal(b, 0.0f))
    {
        if(a > 0.0f)
        {
            return ray::MIN_T;
        }
        else
        {
            return ray::MAX_T;
        }
    }
    else
    {
        return (x - a) / b;
    }
}

float ray::t(const rt::vector& point, const rt::vector::axis axis) const
{
    return ::t(_origin[axis], _direction[axis], point[axis]);
}

float ray::t(const rt::vector& point, const int axis) const
{
    return ::t(_origin[axis], _direction[axis], point[axis]);
}

float ray::t_x(const float x) const
{
    return ::t(_origin.x, _direction.x, x);
}

float ray::t_y(const float y) const
{
    return ::t(_origin.y, _direction.y, y);
}

float ray::t_z(const float z) const
{
    return ::t(_origin.z, _direction.z, z);
}

rt::vector ray::t(const rt::vector& point) const
{
    ZoneScoped;

    rt::vector result;

    result.x = t_x(point.x);
    result.y = t_y(point.y);
    result.z = t_z(point.z);

    return result;
}

ray ray::from_to(const vector& from, const vector& to)
{
    ZoneScoped;
    return {from, (to - from).normalized()};
}
