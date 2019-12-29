#ifndef RAYTRACER_RAY_HPP_INCLUDED
#define RAYTRACER_RAY_HPP_INCLUDED

#include <limits>
#include <raytracer/vector.hpp>

namespace rt
{

class ray
{
public:
    static constexpr float MIN_T = std::numeric_limits<float>::min();
    static constexpr float MAX_T = std::numeric_limits<float>::max();

    ray() = default;
    ray(const rt::vector& origin, const rt::vector& direction);

    const rt::vector& origin() const;
    const rt::vector& direction() const;

    rt::vector point(const float t) const;
    float      t_x(const float x) const;
    float      t_y(const float y) const;
    float      t_z(const float z) const;
    float      t(const rt::vector& point, const rt::vector::axis axis) const;
    float      t(const rt::vector& point, const int axis) const;
    rt::vector t(const rt::vector& point) const;

    static ray from_to(const rt::vector& from, const rt::vector& to);

private:
    rt::vector _origin, _direction;
};

} // namespace rt

#endif // RAYTRACER_RAY_HPP_INCLUDED
