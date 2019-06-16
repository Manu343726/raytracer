cmake_minimum_required(VERSION 3.0)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if(RAYTRACER_SANITIZER)
    message(STATUS "Raytracer project built with -fsanitize=${RAYTRACER_SANITIZER}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=${RAYTRACER_SANITIZER} -fno-omit-frame-pointer -lasan")
endif()

set(RAYTRACER_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
add_library(raytracer-includes INTERFACE)
target_include_directories(raytracer-includes INTERFACE "${RAYTRACER_INCLUDE_DIR}")

set(RAYTRACER_CONANBUILDINFO_FILE "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")

if(EXISTS "${RAYTRACER_CONANBUILDINFO_FILE}")
    include("${RAYTRACER_CONANBUILDINFO_FILE}")
    conan_basic_setup(TARGETS)
else()
    message(WARNING "Conan build info file not found, make sure you have run conan install with the cmake generator")
endif()

add_subdirectory(src)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/.ycm_extra_conf.py.in
    ${CMAKE_CURRENT_SOURCE_DIR}/.ycm_extra_conf.py
)
