#include <cxxopts.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <tinyrefl/api.hpp>

#include <raytracer/canvas.hpp>
#include <raytracer/hitables/scene.hpp>
#include <raytracer/kernel.hpp>
#include <raytracer/runtime/options_parser.hpp>
#include <raytracer/runtime/settings.hpp>
#include <raytracer/hitables/scene.hpp.tinyrefl>
#include <raytracer/kernel.hpp.tinyrefl>
#include <raytracer/runtime/settings.hpp.tinyrefl>

#include <raytracer/debug/profile.hpp>

#include <fstream>

using namespace rt::runtime;

void configure_log_level(const std::string& level)
{
    if(level == "TRACE")
    {
        spdlog::set_level(spdlog::level::trace);
    }
    else if(level == "DEBUG")
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else if(level == "INFO")
    {
        spdlog::set_level(spdlog::level::info);
    }
    else if(level == "WARNING")
    {
        spdlog::set_level(spdlog::level::warn);
    }
    else if(level == "ERROR")
    {
        spdlog::set_level(spdlog::level::err);
    }
    else if(level == "CRITICAL")
    {
        spdlog::set_level(spdlog::level::critical);
    }
    else if(level == "OFF")
    {
        spdlog::set_level(spdlog::level::off);
    }
}

void kernel_runner(const rt::runtime::settings& settings)
{
    rt::canvas canvas{settings.kernel_constants.screen_width,
                      settings.kernel_constants.screen_height};

    settings.dump();

    spdlog::info("Starting...");
    canvas.foreach(
        ::kernel,
        settings.kernel_constants,
        settings.compute_threads(),
        settings.compute_jobs_per_thread());
    spdlog::info("Done. Saving rendering to file...");

    canvas.dump_to_file(settings.output_file);
}

int run(int argc, const char** argv)
{
    auto options = rt::runtime::generate_options<rt::runtime::settings>(
        "raytracer-client", "raytracer lib client");
    // clang-format off
    options.add_options()
        ("help", "Display this help and exit")
        ("c,config-file", "Full path to the configuration JSON file", cxxopts::value<std::string>())
#ifndef RAYTRACER_EASY_PROFILER
        ;
#else
        ("profile-output", "easy_profiler recording output file", cxxopts::value<std::string>());
#endif // RAYTRACER_EASY_PROFILER
    // clang-format on


    const auto  args                = options.parse(argc, argv);
    std::string profile_output_file = "raytracer.prof";

    if(args.count("help"))
    {
        fmt::print("{}\n", options.help());
        return 0;
    }
    else
    {
        if(args.count("profile-output"))
        {
            profile_output_file = args["profile-output"].as<std::string>();
        }

        rt::runtime::settings settings;

        if(args.count("config-file"))
        {
            const std::string filepath = args["config-file"].as<std::string>();

            std::ifstream config_file{filepath};

            if(config_file)
            {
                tinyrefl::json json;
                config_file >> json;

                spdlog::info("loading config file {} ...", filepath);
                settings = tinyrefl::from_json<rt::runtime::settings>(json);
            }
            else
            {
                spdlog::error("cannot open config file {}", filepath);
            }
        }

        rt::runtime::apply_options(args, settings);

        const float default_aspect_ratio =
            static_cast<float>(settings.kernel_constants.screen_width) /
            settings.kernel_constants.screen_height;

        if(!settings.use_custom_aspect_ratio)
        {
            settings.kernel_constants.aspect_ratio = default_aspect_ratio;
        }
        else if(rt::float_not_equal(
                    settings.kernel_constants.aspect_ratio,
                    default_aspect_ratio))
        {
            spdlog::warn(
                "Custom aspect ratio {} is not the correct aspect ratio ({}) for {}x{} resolution",
                settings.kernel_constants.aspect_ratio,
                default_aspect_ratio,
                settings.kernel_constants.screen_width,
                settings.kernel_constants.screen_height);
        }

        if(settings.canvas_follows_screen_resolution)
        {
            settings.kernel_constants.scene.camera.set_aspect_ratio(
                settings.kernel_constants.aspect_ratio);
        }
        else
        {
            if(settings.kernel_constants.aspect_ratio !=
               settings.kernel_constants.scene.camera.aspect_ratio())
            {
                spdlog::warn(
                    "Renderer screen configured with aspect ratio {} but camera was configured with {} aspect ratio, beware of potential aspect ratio or scaling issues",
                    settings.kernel_constants.aspect_ratio,
                    settings.kernel_constants.scene.camera.aspect_ratio());
            }
        }

        configure_log_level(settings.log_level);

        kernel_runner(settings);
        return 0;
    }
}

void* operator new(std::size_t size)
{
    void* ptr = std::malloc(size);
    TracyAlloc(ptr, size);
    return ptr;
}

void operator delete(void* ptr)
{
    TracyFree(ptr);
    std::free(ptr);
}

int main(int argc, const char** argv)
{
    return run(argc, argv);
}
