#include <cxxopts.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <raytracer/canvas.hpp>
#include <raytracer/kernel.hpp>

#include <cstdint>
#include <string>

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

struct runtime_settings
{
    rt::kernel_constants kernel_constants;
    std::size_t threads;
    std::string output_file;

    void dump() const
    {
        spdlog::info("runtime settings:");
        spdlog::info(" - worker threads: {}", threads);
        spdlog::info(" - output file: {}", output_file);
        spdlog::info("constants:");
        spdlog::info(" - iterations: {}", kernel_constants.iterations);
        spdlog::info(" - samples per pixel: {}", kernel_constants.samples_per_pixel);
        spdlog::info(" - screen width: {} pixels", kernel_constants.screen_width);
        spdlog::info(" - screen height: {} pixels", kernel_constants.screen_height);
        spdlog::info(" - aspect ratio: {}", kernel_constants.aspect_ratio);
        spdlog::info("");
    }
};

void kernel_runner(const runtime_settings& settings)
{
    rt::canvas canvas{
        settings.kernel_constants.screen_width,
        settings.kernel_constants.screen_height};

    settings.dump();

    spdlog::info("Starting...");
    canvas.foreach(::kernel, settings.kernel_constants, settings.threads);
    spdlog::info("Done. Saving rendering to file...");

    canvas.dump_to_file(settings.output_file);
}

int run(int argc, const char** argv)
{
    cxxopts::Options options{"raytracer-client", "raytracer lib client"};
    // clang-format off
    options.add_options()
        ("help", "Display this help and exit")
        ("t,threads", "Number of job engine parallel threads", cxxopts::value<std::size_t>()->default_value(std::to_string(std::thread::hardware_concurrency())))
        ("l,log-level", "Logging level (TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL, OFF)", cxxopts::value<std::string>()->default_value("INFO"))
        ("i,iterations", "Number of iterations", cxxopts::value<std::size_t>()->default_value("1"))
        ("s,samples", "Samples per pixel", cxxopts::value<std::size_t>()->default_value("1"))
        ("w,width", "Width of the output image in pixels (default: 800)", cxxopts::value<std::size_t>()->default_value("800"))
        ("h,height", "Height of the ouput image in pixels (default: 600)", cxxopts::value<std::size_t>()->default_value("600"))
        ("aspect-ratio", "Aspect ratio used by the renderer (default: width/height)", cxxopts::value<float>())
        ("f,output-file", "Output image file (default: output.ppm)", cxxopts::value<std::string>()->default_value("output.ppm"));
    // clang-format on

    const auto args = options.parse(argc, argv);

    if(args.count("help"))
    {
        fmt::print("{}\n", options.help());
        return 0;
    }
    else
    {
        if(args.count("log-level"))
        {
            configure_log_level(args["log-level"].as<std::string>());
        }

        runtime_settings settings;

        settings.threads      = args["threads"].as<std::size_t>();
        settings.output_file  = args["output-file"].as<std::string>();

        settings.kernel_constants.iterations        = args["iterations"].as<std::size_t>();
        settings.kernel_constants.samples_per_pixel = args["samples"].as<std::size_t>();
        settings.kernel_constants.screen_width      = args["width"].as<std::size_t>();
        settings.kernel_constants.screen_height     = args["height"].as<std::size_t>();

        if(args.count("aspect-ratio"))
        {
            settings.kernel_constants.aspect_ratio  = args["aspect-ratio"].as<float>();
        }
        else
        {
            spdlog::debug("Using default aspect ratio");

            settings.kernel_constants.aspect_ratio =
                static_cast<float>(settings.kernel_constants.screen_width) /
                                   settings.kernel_constants.screen_height;
        }

        kernel_runner(settings);

        return 0;
    }
}

int main(int argc, const char** argv)
{
    return run(argc, argv);
}
