#include <raytracer/camera.hpp>
#include <raytracer/reflection/from_json.hpp>
#include <raytracer/camera.hpp.tinyrefl>

#include <cmath>
#include <raytracer/debug/profile.hpp>

using namespace rt;

namespace rt
{

camera::camera(
    const rt::vector& position,
    const rt::vector& look_at,
    const rt::vector& up,
    const float       fov,
    const float       aspect_ratio)
    : _position{position}, _look_at{look_at}, _up{up}, _fov{fov}
{
    set_aspect_ratio(aspect_ratio);
}

void camera::set_aspect_ratio(const float aspect_ratio)
{
    const float theta       = _fov * M_PI / 180.0f;
    const float half_height = std::tan(theta / 2.0f);
    const float half_width  = aspect_ratio * half_height;

    const auto w = (_position - _look_at).normalized();
    const auto u = rt::cross_product(_up, w).normalized();
    const auto v = rt::cross_product(w, u);

    _bottom_left_corner = _position - half_width * u - half_height * v - w;
    _horizontal         = 2.0f * half_width * u;
    _vertical           = 2.0f * half_height * v;
    _aspect_ratio       = aspect_ratio;
}

rt::ray camera::ray(const float u, const float v) const
{
    ZoneScoped;

    return rt::ray::from_to(
        _position, _bottom_left_corner + u * _horizontal + v * _vertical);
}

const rt::vector& camera::position() const
{
    return _position;
}

const rt::vector& camera::look_at() const
{
    return _look_at;
}

const rt::vector& camera::up() const
{
    return _up;
}

const rt::vector& camera::bottom_left_corner() const
{
    return _bottom_left_corner;
}

float camera::fov() const
{
    return _fov;
}

float camera::aspect_ratio() const
{
    return _aspect_ratio;
}

void from_json(const tinyrefl::json& json, camera& camera)
{
    camera = *rt::reflection::from_json<rt::camera>(json);
}

} // namespace rt
