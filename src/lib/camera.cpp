#include <raytracer/camera.hpp>
#include <raytracer/reflection/from_json.hpp>
#include <raytracer/camera.hpp.tinyrefl>

using namespace rt;

namespace rt
{

camera::camera(
    const rt::vector& position,
    const rt::vector& look_at,
    const rt::vector& up,
    const float       viewport_width,
    const float       viewport_height,
    const float       fov)
    : _position{position}, _look_at{look_at}, _up{up}, _fov{fov}
{
    set_viewport_size(viewport_width, viewport_height);
}

void camera::set_viewport_size(const float width, const float height)
{
    _bottom_left_corner = vector{
        _look_at.x - (width * 0.5f), _look_at.y - (height * 0.5f), _look_at.z};

    _viewport_width  = width;
    _viewport_height = height;
}

rt::ray camera::ray(const float u, const float v) const
{
    return rt::ray::from_to(
        _position,
        _bottom_left_corner + rt::vector{u * _viewport_width,
                                         v * _viewport_height,
                                         _bottom_left_corner.z});
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

float camera::viewport_width() const
{
    return _viewport_width;
}

float camera::viewport_height() const
{
    return _viewport_height;
}

float camera::fov() const
{
    return _fov;
}

void from_json(const tinyrefl::json& json, camera& camera)
{
    camera = *rt::reflection::from_json<rt::camera>(json);
}

} // namespace rt
