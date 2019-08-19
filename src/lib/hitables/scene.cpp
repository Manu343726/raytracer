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
    float closest_so_far = max_t;
    bool  hit_anything   = false;

    for(const auto& object : objects)
    {
        hit_record tmp_hit;

        if(object->hit(ray, min_t, closest_so_far, tmp_hit))
        {
            if(tmp_hit.t < 0.0f)
            {
                spdlog::warn("scene::hit() negative hit t ({}, min: {}, max: {})",
                    tmp_hit.t, min_t, closest_so_far);
            }

            closest_so_far = tmp_hit.t;
            hit            = tmp_hit;
            hit_anything   = true;
        }
    }

    return hit_anything;
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
    float result = 0.0f;
    const vector center = this->center();

    for(const auto& object : objects)
    {
        result = std::max(result, (object->center() - center).length() - object->radious());
    }

    return result;
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

} // namespace hitables
} // namespace rt
