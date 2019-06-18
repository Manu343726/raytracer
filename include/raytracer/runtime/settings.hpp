#ifndef RAYTRACER_RUNTIME_SETTINGS_HPP_INCLUDED
#define RAYTRACER_RUNTIME_SETTINGS_HPP_INCLUDED

#include <raytracer/kernel.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace rt
{

namespace runtime
{

struct settings
{
    rt::kernel_constants kernel_constants;
    std::size_t          threads     = std::thread::hardware_concurrency();
    std::string          output_file = "output.ppm";
    std::string          log_level   = "INFO";
    std::unordered_map<std::string, std::string> jobs_per_thread;
    std::string default_jobs_per_thread = "all";

    void                     dump() const;
    std::vector<std::size_t> compute_jobs_per_thread() const;

private:
    std::size_t compute_default_jobs_per_thread() const;
    std::size_t compute_thread_jobs(
        const std::string& config, const std::size_t default_jobs) const;
    std::size_t compute_thread_jobs(const std::string& config) const;
    std::size_t total_jobs() const;
};
} // namespace runtime
} // namespace rt

#endif // RAYTRACER_RUNTIME_SETTINGS_HPP_INCLUDED
