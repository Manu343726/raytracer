add_library(raytracer-runtime runtime.cpp)
target_link_libraries(raytracer-runtime
    PUBLIC raytracer-lib
    PRIVATE CONAN_PKG::cxxopts)
target_compile_options(raytracer-runtime INTERFACE -include "${CMAKE_SOURCE_DIR}/include/raytracer/kernel.hpp")
