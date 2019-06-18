add_library(raytracer-runtime
    settings.cpp
    run.cpp)

target_link_libraries(raytracer-runtime
    PUBLIC raytracer-lib
    PRIVATE CONAN_PKG::cxxopts)

target_compile_options(raytracer-runtime INTERFACE -include${CMAKE_SOURCE_DIR}/include/raytracer/kernel.hpp)

find_package(tinyrefl_tool REQUIRED)
tinyrefl_tool(TARGET raytracer-runtime HEADERS
    "${RAYTRACER_INCLUDE_DIR}/raytracer/runtime/settings.hpp")
