#ifndef RAYTRACER_MATERIALS_LAMBERTIAN_HPP_INCLUDED
#define RAYTRACER_MATERIALS_LAMBERTIAN_HPP_INCLUDED

#include <raytracer/material.hpp>

namespace rt
{

namespace materials
{

struct lambertian : rt::material
{
    lambertian(const rt::vector& albedo, const rt::vector& emissive);

    bool scatter(
        const rt::ray&        in,
        const rt::hit_record& hit,
        rt::vector&           attenuation,
        rt::ray&              scattered) const override;

    rt::color emitted() const;

    std::string to_string() const override;

private:
    rt::vector _albedo;
    rt::vector _emissive;
};

} // namespace materials

} // namespace rt

#endif // RAYTRACER_MATERIALS_LAMBERTIAN_HPP_INCLUDED
