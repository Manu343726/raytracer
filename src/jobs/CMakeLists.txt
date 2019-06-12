find_package(Threads REQUIRED)

add_library(raytracer-jobs
    job.cpp
    pool.cpp
    worker.cpp
    engine.cpp
    jobqueue.cpp
    lockedjobqueue.cpp
)

target_link_libraries(raytracer-jobs PUBLIC
    raytracer-includes
    raytracer-lib
    CONAN_PKG::spdlog
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(raytracer-jobs-example example.cpp)
target_link_libraries(raytracer-jobs-example PRIVATE raytracer-jobs)

option(RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK "Simulate work during job scheduler example" OFF)
option(RAYTRACER_JOBS_EXAMPLE_LOG_WORK "Print to stdout when a job in the job scheduler example finishes" OFF)
option(RAYTRACER_JOBS_EXAMPLE_USE_CLOSURE_JOB "Use closures for children jobs in job scheduler example" OFF)

if(RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK)
    target_compile_definitions(raytracer-jobs-example PRIVATE RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK)
endif()

if(RAYTRACER_JOBS_EXAMPLE_LOG_WORK)
    target_compile_definitions(raytracer-jobs-example PRIVATE RAYTRACER_JOBS_EXAMPLE_LOG_WORK)
endif()

if(RAYTRACER_JOBS_EXAMPLE_USE_CLOSURE_JOB)
    target_compile_definitions(raytracer-jobs-example PRIVATE RAYTRACER_JOBS_EXAMPLE_USE_CLOSURE_JOB)
endif()
