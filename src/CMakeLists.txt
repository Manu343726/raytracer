add_subdirectory(jobs)
add_subdirectory(lib)
add_subdirectory(runtime)

function(raytracer_exec NAME)
    add_executable(${NAME} ${NAME}.cpp)
    target_link_libraries(${NAME} PRIVATE raytracer-runtime)
endfunction()

raytracer_exec(raytracer)
raytracer_exec(mandelbrot)
