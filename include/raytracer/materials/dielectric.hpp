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
    dielectric(const float refraction_index, const rt::vector& albedo);

    bool scatter(
        const rt::ray&        in,
        const rt::hit_record& hit,
        rt::vector&           attenuation,
        rt::ray&              scattered) const override;

    std::string to_string() const override;

private:
    float _refraction_index;
    rt::vector _albedo;
};

} // namespace materials

} // namespace rt

#endif // RAYTRACER_MATERIALS_DIELECTRIC_HPP_INCLUDED
