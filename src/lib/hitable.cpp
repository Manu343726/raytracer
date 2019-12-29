#include <raytracer/debug/profile.hpp>
#include <raytracer/hitable.hpp>
#include <raytracer/math.hpp>
#include <raytracer/reflection/factory.hpp>

#include <climits>

namespace rt
{

bool hitable::hit(const ray& ray, hit_record& hit) const
{
    ZoneScoped;

    return this->hit(ray, rt::ray::MIN_T, rt::ray::MAX_T, hit);
}

bool hitable::broad_hit(
    const ray& ray, const float min_t, const float max_t) const
{
    ZoneScoped;

    return bounding_box().contains(ray, min_t, max_t);
}

std::unique_ptr<rt::hitable>
    make_hitable(const std::string& type_name, const tinyrefl::json& args)
{
    return rt::reflection::factory<rt::hitable>(type_name, args);
}

std::string hitable::to_string() const
{
    return "";
}

vector hitable::center() const
{
    return bounding_box().center();
}

float hitable::radious() const
{
    const auto box_size = bounding_box().size();

    return rt::max(box_size.x, box_size.y, box_size.z);
}

std::ostream& operator<<(std::ostream& os, const hitable& hitable)
{
    return os << hitable.to_string();
}
} // namespace rt
