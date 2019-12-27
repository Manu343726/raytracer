#include <raytracer/debug/profile.hpp>
#include <raytracer/hitable.hpp>
#include <raytracer/materials/metal.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace rt;
using namespace rt::materials;

metal::metal(const vector& albedo, const float fuzziness)
    : _albedo{albedo}, _fuzziness{std::min(1.0f, fuzziness)}
{
}

bool metal::scatter(
    const rt::ray&        in,
    const rt::hit_record& hit,
    rt::vector&           attenuation,
    rt::ray&              scattered) const
{
    ZoneScoped;

    scattered   = rt::ray{hit.point,
                        rt::reflect(in.direction().normalized(), hit.normal) +
                            _fuzziness * vector::spheric_random()};
    attenuation = _albedo;

    return true;
}

std::string metal::to_string() const
{
    return fmt::format(
        "rt::materials::metal{{albedo: {}, fuzziness: {}}}",
        _albedo,
        _fuzziness);
}
