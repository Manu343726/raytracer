#include <raytracer/hitable.hpp>
#include <raytracer/materials/lambertian.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace rt;
using namespace rt::materials;

lambertian::lambertian(const vector& albedo) : _albedo{albedo} {}

bool lambertian::scatter(
    const rt::ray&        in,
    const rt::hit_record& hit,
    rt::vector&           attenuation,
    rt::ray&              scattered) const
{
    vector target = hit.point + hit.normal + vector::spheric_random();

    scattered   = ray::from_to(hit.point, target);
    attenuation = _albedo;

    return true;
}

std::string lambertian::to_string() const
{
    return fmt::format("rt::materials::lambertian{{albedo: {}}}", _albedo);
}
