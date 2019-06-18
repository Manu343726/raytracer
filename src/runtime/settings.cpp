#include <ctti/detail/cstring.hpp>
#include <raytracer/runtime/settings.hpp>
#include <spdlog/spdlog.h>

using namespace rt::runtime;

void settings::dump() const
{
    spdlog::info("runtime settings:");
    spdlog::info(" - worker threads: {}", threads);
    spdlog::info(" - output file: {}", output_file);
    spdlog::debug(
        " - default jobs per thread: {} ({})",
        default_jobs_per_thread,
        compute_default_jobs_per_thread());
    spdlog::info(" - max allocated jobs per thread:");

    const auto computed_jobs_per_thread = compute_jobs_per_thread();

    for(std::size_t i = 0; i < threads; ++i)
    {
        auto it = jobs_per_thread.find(fmt::format("thread-{}", i));

        if(it != jobs_per_thread.end())
        {
            spdlog::info(
                "    thread {}: {} ({} jobs)",
                i,
                it->second,
                computed_jobs_per_thread[i]);
        }
        else
        {
            spdlog::info(
                "    thread {}: {} ({} jobs)",
                i,
                default_jobs_per_thread,
                computed_jobs_per_thread[i]);
        }
    }

    spdlog::info("constants:");
    spdlog::info(" - iterations: {}", kernel_constants.iterations);
    spdlog::info(
        " - samples per pixel: {}", kernel_constants.samples_per_pixel);
    spdlog::info(" - screen width: {} pixels", kernel_constants.screen_width);
    spdlog::info(" - screen height: {} pixels", kernel_constants.screen_height);
    spdlog::info(" - aspect ratio: {}", kernel_constants.aspect_ratio);
    spdlog::info("");
}

std::size_t thread_id_from_string(const std::string& thread_id)
{
    constexpr ctti::detail::cstring thread_id_prefix = "thread-";

    if(thread_id.size() > thread_id_prefix.size() &&
       thread_id.substr(0, thread_id_prefix.size()) == thread_id_prefix.str())
    {
        return static_cast<std::size_t>(
            std::atoll(thread_id.substr(thread_id_prefix.size()).c_str()));
    }
    else
    {
        return 0;
    }
}

std::size_t settings::compute_thread_jobs(
    const std::string& config, const std::size_t default_jobs) const
{
    if(config == "all")
    {
        return total_jobs();
    }
    else if(config == "default")
    {
        return default_jobs;
    }
    else if(config.find_last_of('%') == (config.size() - 1))
    {
        const auto percent_str = config.substr(0, config.size() - 1);
        const auto percent     = std::atoll(percent_str.c_str());
        const auto max_jobs = (default_jobs > 0 ? default_jobs : total_jobs());
        const auto jobs = static_cast<std::size_t>(max_jobs * percent / 100.0f);

        spdlog::debug(
            "parsing percent thread jobs \"{}\": {} ({}) percent ({} jobs up to {})",
            config,
            percent_str,
            percent,
            jobs,
            max_jobs);

        return jobs;
    }
    else
    {
        return static_cast<std::size_t>(std::atoll(config.c_str()));
    }
}

std::size_t settings::compute_thread_jobs(const std::string& config) const
{
    return compute_thread_jobs(config, total_jobs());
}

std::size_t settings::compute_default_jobs_per_thread() const
{
    return compute_thread_jobs(default_jobs_per_thread, total_jobs());
}

std::vector<std::size_t> settings::compute_jobs_per_thread() const
{
    std::vector<std::size_t> result(threads, compute_default_jobs_per_thread());

    for(const auto& [key, value] : jobs_per_thread)
    {
        const std::size_t thread_id = thread_id_from_string(key);

        if(thread_id < threads)
        {
            result[thread_id] = compute_thread_jobs(value);
        }
        else
        {
            spdlog::warn(
                "config error: jobs_per_thread contains entry \"{}\" but config has {} threads only",
                key,
                threads);
        }
    }

    return result;
}

std::size_t settings::total_jobs() const
{
    return kernel_constants.screen_width * kernel_constants.screen_height;
}
