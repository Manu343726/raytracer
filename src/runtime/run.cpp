#include <cxxopts.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <tinyrefl/api.hpp>

#include <raytracer/canvas.hpp>
#include <raytracer/kernel.hpp>
#include <raytracer/runtime/options_parser.hpp>
#include <raytracer/runtime/settings.hpp>
#include <raytracer/kernel.hpp.tinyrefl>
#include <raytracer/runtime/settings.hpp.tinyrefl>

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
        settings.threads,
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
        ("c,config-file", "Full path to the configuration JSON file", cxxopts::value<std::string>());
    // clang-format on

    const auto args = options.parse(argc, argv);

    if(args.count("help"))
    {
        fmt::print("{}\n", options.help());
        return 0;
    }
    else
    {
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

        if(args.count("log-level"))
        {
            configure_log_level(args["log-level"].as<std::string>());
        }

        rt::runtime::apply_options(args, settings);

        kernel_runner(settings);

        return 0;
    }
}

int main(int argc, const char** argv)
{
    return run(argc, argv);
}
