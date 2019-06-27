
void kernel(
    const float             x,
    const float             y,
    const kernel_constants& constants,
    color&                  pixel)
{
    rt::ray        eye_to_pixel = constants.scene.camera.ray(x, y);
    rt::hit_record hit_record;

    spdlog::debug(
        "ray(x: {}, y: {}) -> (origin: {}, direction: {})",
        x,
        y,
        eye_to_pixel.origin(),
        eye_to_pixel.direction());

    if(constants.scene.hit(eye_to_pixel, hit_record))
    {
        pixel = (hit_record.normal + vector{1.0f, 1.0f, 1.0f}) * 0.5f;
    }
}
