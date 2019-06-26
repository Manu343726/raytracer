#ifndef RAYTRACER_SCENE_HPP_INCLUDED
#define RAYTRACER_SCENE_HPP_INCLUDED

#include <raytracer/camera.hpp>
#include <raytracer/hitable.hpp>

#include <tinyrefl/api.hpp>

namespace rt
{

struct scene : public rt::hitable
{
    rt::camera                                camera;
    std::vector<std::unique_ptr<rt::hitable>> objects;

    bool
        hit(const rt::ray&  ray,
            const float     min_t,
            const float     max_t,
            rt::hit_record& hit) const override;
};

void from_json(const tinyrefl::json& json, scene& scene);

} // namespace rt

#endif // RAYTRACER_SCENE_HPP_INCLUDED
