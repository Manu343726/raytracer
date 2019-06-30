
color trace(const ray& ray, const hitable& world, std::size_t depth_left)
{
    rt::hit_record hit_record;

    if(world.hit(ray, 0.0f, FLT_MAX, hit_record))
    {
        rt::ray     scattered;
        vector      attenuation;
        const auto* material = hit_record.material;

        if(depth_left > 0 &&
           material->scatter(ray, hit_record, attenuation, scattered))
        {
            return attenuation * trace(scattered, world, depth_left - 1);
        }
        else
        {
            return color::rgb(0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        vector unit_direction = ray.direction().normalized();

        const float t = 0.5f * (unit_direction.y + 1.0f);

        return (1.0f - t) * color::rgb(1.0f, 1.0f, 1.0f) +
               t * color::rgb(0.5f, 0.7f, 1.0f);
    }
}

void kernel(
    const float             x,
    const float             y,
    const kernel_constants& constants,
    color&                  pixel)
{
    rt::ray eye_to_pixel = constants.scene.camera.ray(x, y);

    pixel = trace(eye_to_pixel, constants.scene, constants.iterations);
}
