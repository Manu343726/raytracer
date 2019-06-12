add_library(raytracer-lib
    math.cpp
    vector.cpp
)

add_subdirectory(jobs)

target_link_libraries(raytracer-lib
    PUBLIC raytracer-includes)
