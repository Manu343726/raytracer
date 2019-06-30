#ifndef RAYTRACER_MATERIALS_METAL_HPP_INCLUDED
#define RAYTRACER_MATERIALS_METAL_HPP_INCLUDED

#include <raytracer/material.hpp>

namespace rt
{

namespace materials
{

struct metal : rt::material
{
    metal(const rt::vector& albedo, const float fuzziness = 0.0f);

    bool scatter(
        const rt::ray&        in,
        const rt::hit_record& hit,
        rt::vector&           attenuation,
        rt::ray&              scattered) const override;

    std::string to_string() const override;

private:
    rt::vector _albedo;
    float      _fuzziness;
};

} // namespace materials

} // namespace rt

#endif // RAYTRACER_MATERIALS_METAL_HPP_INCLUDED
