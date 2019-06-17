add_library(raytracer-lib
    math.cpp
    vector.cpp
    canvas.cpp
    ray.cpp
)

target_link_libraries(raytracer-lib
    PUBLIC raytracer-includes
    PUBLIC CONAN_PKG::spdlog
    PRIVATE raytracer-jobs)
