#ifndef RAYTRACER_KERNEL_HPP_INCLUDED
#define RAYTRACER_KERNEL_HPP_INCLUDED

#include <raytracer/math.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/vector.hpp>

#include <spdlog/spdlog.h>

namespace rt
{

struct kernel_constants
{
    // clang-format off

    [[
        rt::short_name("i"),
        rt::description("Number of iterations (default: 1)")
    ]]
    std::size_t iterations = 1;

    [[
        rt::short_name("s"),
        rt::name("samples"),
        rt::description("Semples per pixel (default: 1)")
    ]]
    std::size_t samples_per_pixel = 1;

    [[
        rt::short_name("w"),
        rt::name("width"),
        rt::description("Width of the output image in pixels (default: 800)")
    ]]
    std::size_t screen_width = 800;

    [[
        rt::short_name("h"),
        rt::name("height"),
        rt::description("Height of the output image in pixels (default: 600)")
    ]]
    std::size_t screen_height = 600;

    [[rt::description(
        "Aspect ratio used by the renderer (default: width/height)")]]
    float aspect_ratio = 800.0f / 600.0f;

    // clang-format on
};

} // namespace rt

void kernel(
    const float                 x,
    const float                 y,
    const rt::kernel_constants& constants,
    rt::color&                  pixel);

using namespace rt;

#endif // RAYTRACER_KERNEL_HPP_INCLUDED
