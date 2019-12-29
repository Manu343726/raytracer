#ifndef RAYTRACER_BOX_HPP_INCLUDED
#define RAYTRACER_BOX_HPP_INCLUDED

#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

namespace rt
{

struct box
{
    box(const rt::vector& min, const rt::vector& max);
    static box centered(const rt::vector& center, const rt::vector& radious);
    static box centered(const rt::vector& center, const float radious);
    static box infinite();
    static box nil();

    bool contains(const rt::vector& point) const;
    bool contains(const rt::ray& ray) const;
    bool contains(
        const rt::ray& ray, const float min_t, const float max_t) const;

    const rt::vector& min() const;
    const rt::vector& max() const;
    rt::vector        center() const;
    rt::vector        size() const;

private:
    rt::vector _min, _max;
};

box surrounding_box(const box& lhs, const box& rhs);

} // namespace rt

#endif // RAYTRACER_BOX_HPP_INCLUDED
