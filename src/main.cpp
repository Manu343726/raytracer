#include <raytracer/canvas.hpp>
#include <spdlog/spdlog.h>
#include <cassert>
#include <complex>

using namespace rt;

int main()
{
    canvas canvas{2560,1440};

    canvas.foreach([](const float x, const float y, const float aspect_ratio, color& pixel)
    {
        using complex = std::complex<float>;

        complex origin{(x - 0.7f)*aspect_ratio*2.0f, (y - 0.5f)*2.0f};
        complex z{0.0f, 0.0f};

        constexpr int max_iterations = 1000;
        int i = 0;

        while(i < max_iterations && std::abs(z) < 2.0f)
        {
            z = z*z + origin;
            i++;
        }

        const float gradient_value = static_cast<float>(max_iterations - i) / max_iterations;

        pixel = color::rgb(gradient_value, gradient_value, gradient_value);
    });

    canvas.dump_to_file("raytracer.ppm");
}
