#include <raytracer/reflection/factory.hpp>
#include <raytracer/reflection/from_json.hpp>
#include <raytracer/utils.hpp>

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

// include all types that should be supported by the factory
#include <raytracer/camera.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/camera.hpp.tinyrefl>
#include <raytracer/sphere.hpp.tinyrefl>

namespace rt
{

namespace reflection
{
factory_instance_ptr
    factory(const std::string& type, const tinyrefl::json& args)
{
    factory_instance_ptr result;

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

        result = factory_instance_ptr{
            rt::reflection::from_json<class_type>(args).release(),
            +[](void* ptr) { rt::utils::destroy<class_type>(ptr); }};
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
