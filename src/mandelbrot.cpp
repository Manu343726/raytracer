
void kernel(
    const float x, const float y, const kernel_constants& constants, color& pixel)
{
    complex origin{(x - 0.7f) * constants.aspect_ratio * 2.0f, (y - 0.5f) * 2.0f};
    complex z{0.0f, 0.0f};

    int i = 0;
    const int max_iterations = static_cast<int>(constants.iterations);

    while(i < max_iterations && std::abs(z) < 2.0f)
    {
        z = z * z + origin;
        i++;
    }

    if(i < max_iterations)
    {
        pixel = color::hsv(static_cast<float>(i) / max_iterations);
    }
}
