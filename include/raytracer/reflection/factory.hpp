#ifndef RAYTRACER_REFLECTION_FACTORY_HPP_INCLUDED
#define RAYTRACER_REFLECTION_FACTORY_HPP_INCLUDED

#include <tinyrefl/api.hpp>

#include <raytracer/reflection/from_json.hpp>
#include <raytracer/utils.hpp>

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include <cassert>
#include <memory>

// include all types that should be supported by the factory
#include <raytracer/camera.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/camera.hpp.tinyrefl>
#include <raytracer/sphere.hpp.tinyrefl>

namespace rt
{

namespace reflection
{

template<typename Interface>
std::unique_ptr<Interface>
    factory(const std::string& type, const tinyrefl::json& args)
{
    std::unique_ptr<Interface> result;

    TINYREFL_VISIT_ENTITIES(tinyrefl::class_visitor([&](const auto& class_) {
        if(type != class_.full_name().str())
        {
            spdlog::trace(
                "make_hitable(type: \"{}\"): Class {} discarded",
                type,
                class_.full_name());
            return;
        }

        using class_type = typename std::decay_t<decltype(class_)>::class_type;

        if constexpr(std::is_base_of<Interface, class_type>())
        {
            result.reset(rt::reflection::from_json<class_type>(args).release());
        }
        else
        {
            spdlog::trace(
                "make_hitable(type: \"{}\"): Class {} discarded (wrong class hierarchy)",
                type,
                class_.full_name());
        }
    }));

    if(result == nullptr)
    {
        spdlog::error(
            "Could not create hitable instance of type \"{}\" (Wrong class name?)",
            type);
    }

    return result;
}
} // namespace reflection

} // namespace rt

#endif // RAYTRACER_REFLECTION_FACTORY_HPP_INCLUDED
