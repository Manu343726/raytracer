#include <raytracer/reflection/from_json.hpp>
#include <raytracer/scene.hpp>

#include <raytracer/camera.hpp>
#include <raytracer/camera.hpp.tinyrefl>

#include <spdlog/spdlog.h>

namespace rt
{

bool scene::hit(
    const rt::ray&  ray,
    const float     min_t,
    const float     max_t,
    rt::hit_record& hit) const
{
    for(const auto& object : objects)
    {
        if(object->hit(ray, min_t, max_t, hit))
        {
            return true;
        }
    }

    return false;
}

void from_json(const tinyrefl::json& json, scene& scene)
{
    const auto& camera  = json["camera"];
    const auto& objects = json["objects"];

    scene.camera = *rt::reflection::from_json<rt::camera>(camera);

    scene.objects.reserve(objects.size());

    for(const auto& object : objects)
    {
        const auto& type_name = object["type"];
        const auto& args      = object["args"];

        auto hitable = rt::make_hitable(type_name, args);

        if(hitable != nullptr)
        {
            scene.objects.push_back(std::move(hitable));
        }
        else
        {
            spdlog::warn(
                "Scene object {} ignored (wrong args? wrong type name?)",
                object.dump());
        }
    }
}

} // namespace rt
