#ifndef RAYTRACER_HITABLES_SPHERE_HPP_INCLUDED
#define RAYTRACER_HITABLES_SPHERE_HPP_INCLUDED

#include <raytracer/hitable.hpp>
#include <raytracer/material.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

#include <optional>

namespace rt
{

namespace hitables
{

class sphere : public rt::hitable
{
public:
    sphere(
        const rt::vector&             center,
        const float                   radious,
        std::unique_ptr<rt::material> material);

    const rt::vector& center() const;
    float             radious() const;

    std::optional<float>      hit(const rt::ray& ray) const;
    std::optional<rt::vector> hit_point(const rt::ray& ray) const;

    bool
        hit(const rt::ray&  ray,
            const float     min_t,
            const float     max_t,
            rt::hit_record& hit) const override;

    std::string to_string() const override;

private:
    rt::vector                    _center;
    float                         _radious;
    std::unique_ptr<rt::material> _material;
};

} // namespace hitables
} // namespace rt

#endif // RAYTRACER_HITABLES_SPHERE_HPP_INCLUDED
