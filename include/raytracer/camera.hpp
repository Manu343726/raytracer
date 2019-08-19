#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

namespace rt
{

struct camera
{
    camera() = default;

    camera(
        const rt::vector& position,
        const rt::vector& look_at,
        const rt::vector& up,
        const float       fov,
        const float       aspect_ratio);

    rt::ray ray(const float u, const float v) const;

    const rt::vector& position() const;
    const rt::vector& look_at() const;
    const rt::vector& up() const;
    const rt::vector& bottom_left_corner() const;
    float             fov() const;
    float             aspect_ratio() const;

    void set_aspect_ratio(const float aspect_ratio);

private:
    rt::vector _position;
    rt::vector _look_at;
    rt::vector _up;
    rt::vector _bottom_left_corner;
    rt::vector _horizontal, _vertical;
    float      _fov;
    float      _aspect_ratio;
};

void from_json(const tinyrefl::json& json, camera& camera);

} // namespace rt

#endif // RAYTRACER_CAMERA_HPP
