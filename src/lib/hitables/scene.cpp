#include <raytracer/debug/profile.hpp>
#include <raytracer/hitables/scene.hpp>
#include <raytracer/reflection/from_json.hpp>

#include <raytracer/camera.hpp>
#include <raytracer/camera.hpp.tinyrefl>

#include <spdlog/spdlog.h>

namespace rt
{

namespace hitables
{

bool scene::hit(
    const rt::ray&  ray,
    const float     min_t,
    const float     max_t,
    rt::hit_record& hit) const
{
    ZoneNamedN(SceneHit, "scene::hit", true);

    bool result = kdtree->hit(ray, min_t, max_t, hit);

    if(result)
    {
        TracyMessageL("Object hit");
    }
    else
    {
        TracyMessageL("All objects missed");
    }

    return result;
}

vector scene::center() const
{
    vector result{0.0f, 0.0f, 0.0f};

    for(const auto& object : objects)
    {
        result += object->center();
    }

    result /= objects.size();

    return result;
}

float scene::radious() const
{
    float        result = 0.0f;
    const vector center = this->center();

    for(const auto& object : objects)
    {
        result = std::max(
            result, (object->center() - center).length() - object->radious());
    }

    return result;
}

box scene::bounding_box() const
{
    ZoneScoped;

    box result = box::nil();

    for(const auto& object : objects)
    {
        result = rt::surrounding_box(result, object->bounding_box());
    }

    return result;
}

void from_json(const tinyrefl::json& json, scene& scene)
{
    ZoneNamedN(FromJson, "scene::from_json", true);

    spdlog::info("Loading scene...");

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

    spdlog::info("Building kd-tree...");

    scene.kdtree = std::make_unique<rt::hitables::kdtree>(
        scene.objects.begin(), scene.objects.end());
}

} // namespace hitables
} // namespace rt
