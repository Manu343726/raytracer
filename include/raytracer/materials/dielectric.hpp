#ifndef RAYTRACER_MATERIALS_DIELECTRIC_HPP_INCLUDED
#define RAYTRACER_MATERIALS_DIELECTRIC_HPP_INCLUDED

#include <raytracer/material.hpp>
#include <raytracer/vector.hpp>

namespace rt
{

namespace materials
{

struct dielectric : rt::material
{
    dielectric(const float refraction_index, const rt::vector& albedo, const rt::vector& emissive);

    bool scatter(
        const rt::ray&        in,
        const rt::hit_record& hit,
        rt::vector&           attenuation,
        rt::ray&              scattered) const override;

    std::string to_string() const override;

    rt::vector emitted() const override;

private:
    float _refraction_index;
    rt::vector _albedo;
    rt::vector _emissive;
};

} // namespace materials

} // namespace rt

#endif // RAYTRACER_MATERIALS_DIELECTRIC_HPP_INCLUDED
