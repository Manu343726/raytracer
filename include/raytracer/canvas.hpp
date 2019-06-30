#ifndef RAYTRACER_CANVAS_HPP_INCLUDED
#define RAYTRACER_CANVAS_HPP_INCLUDED

#include <raytracer/kernel.hpp>
#include <raytracer/vector.hpp>

#include <string>
#include <thread>
#include <vector>

namespace rt
{

class canvas
{
public:
    canvas(const std::size_t width, const std::size_t height);

    std::size_t width() const;
    std::size_t height() const;
    std::size_t pixel_count() const;

    rt::color& pixel(const std::size_t row, const std::size_t column);
    const rt::color&
        pixel(const std::size_t row, const std::size_t column) const;

    void dump_to_file(const std::string& filename) const;

    using pixel_function = void (*)(
        const float /*x*/,
        const float /*y*/,
        const rt::kernel_constants& /*constants*/,
        color& /*pixel*/);

    void foreach(
        pixel_function          function,
        const kernel_constants& constants,
        const std::size_t       threads = std::thread::hardware_concurrency(),
        const std::vector<std::size_t>& jobsPerThread = {});

private:
    const std::size_t      _width, _height;
    std::vector<rt::color> _canvas;
};

} // namespace rt

#endif // RAYTRACER_CANVAS_HPP_INCLUDED
