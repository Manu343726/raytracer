#ifndef RAYTRACER_RAY_HPP_INCLUDED
#define RAYTRACER_RAY_HPP_INCLUDED

#include <raytracer/vector.hpp>

namespace rt
{

class ray
{
public:
    ray() = default;
    ray(const rt::vector& origin, const rt::vector& direction);

    const rt::vector& origin() const;
    const rt::vector& direction() const;

    rt::vector point(const float t) const;

    static ray from_to(const rt::vector& from, const rt::vector& to);

private:
    rt::vector _origin, _direction;
};

} // namespace rt

#endif // RAYTRACER_RAY_HPP_INCLUDED
