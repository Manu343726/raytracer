#ifndef RAYTRACER_KERNEL_HPP_INCLUDED
#define RAYTRACER_KERNEL_HPP_INCLUDED

#include <raytracer/math.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/vector.hpp>

#include <spdlog/spdlog.h>

void kernel(
    const float x, const float y, const float aspect_ratio, rt::color& pixel);

using namespace rt;

#endif // RAYTRACER_KERNEL_HPP_INCLUDED
