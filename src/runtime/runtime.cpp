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

void kernel_runner(
    std::size_t        width,
    std::size_t        height,
    std::size_t        threads,
    const std::string& output_file)
{
    rt::canvas canvas{width, height};

    canvas.foreach(::kernel, threads);

    canvas.dump_to_file(output_file);
}

int run(int argc, const char** argv)
{
    cxxopts::Options options{"raytracer-client", "raytracer lib client"};
    // clang-format off
    options.add_options()
        ("help", "Display this help and exit")
        ("t,threads", "Number of job engine parallel threads", cxxopts::value<std::size_t>()->default_value(std::to_string(std::thread::hardware_concurrency())))
        ("l,log-level", "Logging level (TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL, OFF)", cxxopts::value<std::string>()->default_value("INFO"))
        ("w,width", "Width of the output image in pixels (default: 800)", cxxopts::value<std::size_t>()->default_value("800"))
        ("h,height", "Height of the ouput image in pixels (default: 600)", cxxopts::value<std::size_t>()->default_value("600"))
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

        kernel_runner(
            args["width"].as<std::size_t>(),
            args["height"].as<std::size_t>(),
            args["threads"].as<std::size_t>(),
            args["output-file"].as<std::string>());

        return 0;
    }
}

int main(int argc, const char** argv)
{
    return run(argc, argv);
}
