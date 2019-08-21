#include <raytracer/hitable.hpp>
#include <raytracer/materials/dielectric.hpp>
#include <raytracer/math.hpp>


#include <fmt/ostream.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

using namespace rt;
using namespace rt::materials;

float schlick(const float cosine, const float refraction_index)
{
    float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
    r0       = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
}

dielectric::dielectric(const float refraction_index, const vector& albedo, const vector& emissive)
    : _refraction_index{refraction_index}, _albedo{albedo}, _emissive{emissive}
{
}

vector dielectric::emitted() const
{
    return _emissive;
}

bool dielectric::scatter(
    const rt::ray&        in,
    const rt::hit_record& hit,
    rt::vector&           attenuation,
    rt::ray&              scattered) const
{
    attenuation = _albedo;

    float  cosine;
    vector outward_normal;
    float  index;

    const float dot = dot_product(in.direction(), hit.normal);

    if(dot > 0.0f)
    {
        outward_normal = -hit.normal;
        index          = _refraction_index;
        cosine         = _refraction_index * dot;
    }
    else
    {
        outward_normal = hit.normal;
        index          = 1.0f / _refraction_index;
        cosine         = -dot;
    }

    const auto refracted = refract(in.direction(), outward_normal, index);
    float      reflected_ray_probability;

    if(refracted)
    {
        reflected_ray_probability = schlick(cosine, _refraction_index);
    }
    else
    {
        reflected_ray_probability = 1.0f;
    }

    if(rt::random() < reflected_ray_probability)
    {
        scattered = rt::ray(hit.point, reflect(in.direction(), hit.normal));
    }
    else
    {
        scattered = rt::ray(hit.point, refracted.value());
    }

    return true;
}

std::string dielectric::to_string() const
{
    return fmt::format(
        "rt::materials::dielectric{{refraction_index: {}}}", _refraction_index);
}
