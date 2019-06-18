#ifndef RAYTRACER_CANVAS_HPP_INCLUDED
#define RAYTRACER_CANVAS_HPP_INCLUDED

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

    template<typename Constants>
    using generic_pixel_function = void (*)(
        const float /*x*/,
        const float /*y*/,
        Constants /*constants*/,
        color& /*pixel*/);

    using pixel_function = generic_pixel_function<const void*>;

    void foreach(
        pixel_function    function,
        const void* constants = nullptr,
        const std::size_t threads = std::thread::hardware_concurrency());

    template<typename Constants>
    void foreach(
        generic_pixel_function<const Constants&> function,
        const Constants& constants,
        std::size_t threads = std::thread::hardware_concurrency())
    {
        struct data_t
        {
            generic_pixel_function<const Constants&> function;
            const Constants* constants;
        };

        const data_t data{function, &constants};

        foreach([](const float x, const float y, const void* payload, rt::color& pixel)
        {
            const auto& data = *reinterpret_cast<const data_t*>(payload);
            data.function(x, y, *data.constants, pixel);
        }, &data, threads);
    }


private:
    const std::size_t      _width, _height;
    std::vector<rt::color> _canvas;
};

} // namespace rt

#endif // RAYTRACER_CANVAS_HPP_INCLUDED
