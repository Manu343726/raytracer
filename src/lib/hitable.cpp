#include <raytracer/hitable.hpp>
#include <raytracer/reflection/factory.hpp>

namespace rt
{

std::unique_ptr<rt::hitable>
    make_hitable(const std::string& type_name, const tinyrefl::json& args)
{
    return rt::reflection::factory<rt::hitable>(type_name, args);
}

std::string hitable::to_string() const
{
    return "";
}

std::ostream& operator<<(std::ostream& os, const hitable& hitable)
{
    return os << hitable.to_string();
}
} // namespace rt
