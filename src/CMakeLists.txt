add_library(raytracer-lib
    math.cpp
    vector.cpp
    canvas.cpp
    ray.cpp
)

add_subdirectory(jobs)

target_link_libraries(raytracer-lib
    PUBLIC raytracer-includes
    PUBLIC CONAN_PKG::spdlog
    PRIVATE raytracer-jobs)

add_executable(raytracer main.cpp)
target_link_libraries(raytracer
    PRIVATE raytracer-lib)
