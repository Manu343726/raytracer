#include <raytracer/debug/profile.hpp>
#include <raytracer/hitables/sphere.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace rt;
using namespace rt::hitables;

sphere::sphere(
    const vector&                 center,
    const float                   radious,
    std::unique_ptr<rt::material> material)
    : _center{center}, _radious{radious}, _material{std::move(material)}
{
}

std::string sphere::to_string() const
{
    return fmt::format(
        "rt::sphere{{center: {}, radious: {}, material: {}}}",
        _center,
        _radious,
        _material->to_string());
}

vector sphere::center() const
{
    return _center;
}

float sphere::radious() const
{
    return _radious;
}

std::optional<float> sphere::hit(const ray& ray) const
{
    ZoneScoped;
    vector origin_center = ray.origin() - _center;

    const float a = rt::dot_product(ray.direction(), ray.direction());
    const float b = 2.0f * rt::dot_product(origin_center, ray.direction());
    const float c =
        rt::dot_product(origin_center, origin_center) - (_radious * _radious);
    const float discriminant = b * b - 4.0f * a * c;

    if(discriminant >= 0.0f)
    {
        return std::make_optional((-b - std::sqrt(discriminant)) / (2.0f * a));
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<vector> sphere::hit_point(const ray& ray) const
{
    auto hit_param = hit(ray);

    if(hit_param)
    {
        return std::make_optional(ray.point(hit_param.value()));
    }
    else
    {
        return std::nullopt;
    }
}

bool sphere::hit(
    const ray& ray, const float min_t, const float max_t, hit_record& hit) const
{
    ZoneScoped;

    const auto t = this->hit(ray);

    if(t && min_t <= t && t <= max_t)
    {
        ZoneNamedN(HitSuccess, "hit successful return data", true);

        hit.point    = ray.point(t.value());
        hit.normal   = (hit.point - _center).normalized();
        hit.material = _material.get();
        hit.t        = t.value();
        hit.object   = this;

        return true;
    }
    else
    {
        return false;
    }
}
