#include <raytracer/debug/profile.hpp>
#include <raytracer/hitable.hpp>
#include <raytracer/materials/lambertian.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace rt;
using namespace rt::materials;

lambertian::lambertian(const vector& albedo, const vector& emissive)
    : _albedo{albedo}, _emissive{emissive}
{
}

bool lambertian::scatter(
    const rt::ray&        in,
    const rt::hit_record& hit,
    rt::vector&           attenuation,
    rt::ray&              scattered) const
{
    ZoneScoped;

    vector target = hit.point + hit.normal + vector::spheric_random();

    scattered   = ray::from_to(hit.point, target);
    attenuation = _albedo;

    return true;
}

color lambertian::emitted() const
{
    return _emissive;
}

std::string lambertian::to_string() const
{
    return fmt::format(
        "rt::materials::lambertian{{albedo: {}, emitted: {}}}",
        _albedo,
        _emissive);
}
