
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

    if(constants.scene.hit(eye_to_pixel, -10000.0f, 10000.0f, hit_record))
    {
        pixel = hit_record.normal;
    }
}
