#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fstream>
#include <raytracer/canvas.hpp>
#include <raytracer/jobs/engine.hpp>

using namespace rt;

canvas::canvas(const std::size_t width, const std::size_t height)
    : _width{width},
      _height{height},
      _canvas{pixel_count(), color::rgb(0.0f, 0.0f, 0.0f)}
{
}

color& canvas::pixel(const std::size_t row, const std::size_t column)
{
    return _canvas[row * _width + column];
}

const color&
    canvas::pixel(const std::size_t row, const std::size_t column) const
{
    return _canvas[row * _width + column];
}

void canvas::dump_to_file(const std::string& filename) const
{
    std::ofstream os{filename, std::ios_base::out};

    // PPM header
    fmt::print(
        os,
        "P3\n"
        "{} {}\n"
        "255\n",
        _width,
        _height);

    for(std::size_t row = 0; row < _height; ++row)
    {
        for(std::size_t column = 0; column < _width; ++column)
        {
            const auto& pixel = this->pixel(row, column);

            fmt::print(
                os,
                "{:>3} {:>3} {:>3} ",
                static_cast<std::uint8_t>(255 * pixel.r),
                static_cast<std::uint8_t>(255 * pixel.g),
                static_cast<std::uint8_t>(255 * pixel.b));
        }

        fmt::print(os, "\n");
    }
}

std::size_t canvas::width() const
{
    return _width;
}

std::size_t canvas::height() const
{
    return _height;
}

std::size_t canvas::pixel_count() const
{
    return _width * _height;
}

void canvas::foreach(canvas::pixel_function function, const void* constants, const std::size_t threads)
{
    rt::jobs::Engine engine{threads, pixel_count()};
    const float      x_ratio      = 1.0f / _width;
    const float      y_ratio      = 1.0f / _height;

    auto* worker = engine.threadWorker();

    auto* root = worker->pool().createJob([](rt::jobs::Job&) {});

    for(std::size_t row = 0; row < _height; ++row)
    {
        for(std::size_t column = 0; column < _width; ++column)
        {
            auto&       pixel = this->pixel(row, column);
            const float x     = column * x_ratio;
            const float y     = row * y_ratio;

            auto* pixelJob = worker->pool().createClosureJobAsChild(
                [function, x, y, &pixel, constants](rt::jobs::Job& job) {
                    function(x, y, constants, pixel);
                },
                root);

            worker->submit(pixelJob);
        }
    }

    worker->submit(root);
    worker->wait(root);
}
