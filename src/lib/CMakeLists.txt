add_library(raytracer-lib
    segment.cpp
    box.cpp
    math.cpp
    vector.cpp
    canvas.cpp
    ray.cpp
    camera.cpp
    hitable.cpp
    hitables/kdtree.cpp
    hitables/scene.cpp
    hitables/sphere.cpp
    material.cpp
    materials/lambertian.cpp
    materials/metal.cpp
    materials/dielectric.cpp
)

target_link_libraries(raytracer-lib
    PUBLIC raytracer-includes
    PUBLIC CONAN_PKG::spdlog
    PUBLIC CONAN_PKG::range-v3
    PRIVATE raytracer-jobs)

find_package(tinyrefl_tool REQUIRED)
tinyrefl_tool(TARGET raytracer-lib HEADERS
    "${RAYTRACER_INCLUDE_DIR}/raytracer/kernel.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/camera.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/hitables/sphere.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/hitables/scene.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/material.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/materials/lambertian.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/materials/metal.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/materials/dielectric.hpp"
    LOG_LEVEL TRACE)
