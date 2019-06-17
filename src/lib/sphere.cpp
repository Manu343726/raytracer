#include <raytracer/sphere.hpp>

using namespace rt;

sphere::spehere(const vector& center, const float radious)
    : _center{center}, _radious{radious}
{
}

const vector& sphere::center() const
{
    return _center;
}

float sphere::radious() const
{
    return _radious;
}

std::optional<float> sphere::hit(const ray& ray) const
{
    vector origin_center = ray.origin() - _center;

    const float a = ray.direction() * ray.direction();
    const float b = 2.0f * (origin_center * ray.direction());
    const float c = (origin_center * origin_center) - (_radious * _radious);
    const float discriminant = b * b - 4.0f * a * c;

    if(discriminant > 0.0f)
    {
        return std::make_optional(-b * std::sqrt(discriminant) * 0.5f / a);
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
