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
    std::size_t iterations        = 1;
    std::size_t samples_per_pixel = 1;
    std::size_t screen_width      = 800;
    std::size_t screen_height     = 600;
    float       aspect_ratio      = 800.0f / 600.0f;
};

} // namespace rt

void kernel(
    const float                 x,
    const float                 y,
    const rt::kernel_constants& constants,
    rt::color&                  pixel);

using namespace rt;

#endif // RAYTRACER_KERNEL_HPP_INCLUDED
