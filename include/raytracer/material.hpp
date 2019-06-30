#ifndef RAYTRACER_MATERIAL_HPP_INCLUDED
#define RAYTRACER_MATERIAL_HPP_INCLUDED

#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

#include <string>

namespace rt
{

struct hit_record;

struct material
{
    virtual ~material() = default;

    virtual bool scatter(
        const rt::ray&        in,
        const rt::hit_record& hit,
        rt::vector&           attenuation,
        rt::ray&              scattered) const = 0;

    virtual std::string to_string() const;
};

} // namespace rt

#endif // RAYTRACER_MATERIAL_HPP_INCLUDED
