
void kernel(
    const float x, const float y, const float aspect_ratio, color& pixel)
{
    complex origin{(x - 0.7f) * aspect_ratio * 2.0f, (y - 0.5f) * 2.0f};
    complex z{0.0f, 0.0f};

    constexpr int max_iterations = 100;
    int           i              = 0;

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
