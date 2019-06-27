add_library(raytracer-lib
    math.cpp
    vector.cpp
    canvas.cpp
    ray.cpp
    hitable.cpp
    scene.cpp
    sphere.cpp
    camera.cpp
)

target_link_libraries(raytracer-lib
    PUBLIC raytracer-includes
    PUBLIC CONAN_PKG::spdlog
    PRIVATE raytracer-jobs)

find_package(tinyrefl_tool REQUIRED)
tinyrefl_tool(TARGET raytracer-lib HEADERS
    "${RAYTRACER_INCLUDE_DIR}/raytracer/kernel.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/sphere.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/scene.hpp"
    "${RAYTRACER_INCLUDE_DIR}/raytracer/camera.hpp"
    LOG_LEVEL TRACE)
