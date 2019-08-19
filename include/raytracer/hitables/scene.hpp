#ifndef RAYTRACER_HITABLES_SCENE_HPP_INCLUDED
#define RAYTRACER_HITABLES_SCENE_HPP_INCLUDED

#include <raytracer/camera.hpp>
#include <raytracer/hitable.hpp>

#include <tinyrefl/api.hpp>

namespace rt
{

namespace hitables
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

    rt::vector center() const override;
    float radious() const override;

    using hitable::hit;
};

void from_json(const tinyrefl::json& json, scene& scene);

} // namespace hitables
} // namespace rt

#endif // RAYTRACER_HITABLES_SCENE_HPP_INCLUDED
