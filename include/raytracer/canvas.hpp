#ifndef RAYTRACER_CANVAS_HPP_INCLUDED
#define RAYTRACER_CANVAS_HPP_INCLUDED

#include <raytracer/vector.hpp>
#include <vector>
#include <string>

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
    const rt::color& pixel(const std::size_t row, const std::size_t column) const;

    void dump_to_file(const std::string& filename) const;

    using pixel_function =
        void(*)(const float /*x*/, const float /*y*/, const float /*aspect_ratio*/, color& /*pixel*/);

    void foreach(pixel_function function);

private:
    const std::size_t _width, _height;
    std::vector<rt::color> _canvas;
};

}

#endif // RAYTRACER_CANVAS_HPP_INCLUDED
