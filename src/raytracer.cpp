
color probe_lights(
    const rt::ray&          ray,
    const rt::hit_record&   hit_record,
    const kernel_constants& constants)
{
    auto result = color::rgb(0.0f, 0.0f, 0.0f);
    return result;

    for(const auto& light : constants.scene.objects)
    {
        rt::hit_record light_hit_record;

        // Sample light by randomly tracing a ray from the input
        // hit towards the light object
        const rt::ray light_ray{
            hit_record.point,
            rt::vector::hemispheric_random(
                (light->center() - hit_record.point).normalized())};

        if(constants.scene.hit(
               light_ray,
               constants.min_hit_range,
               constants.max_hit_range,
               light_hit_record) &&
           light.get() == light_hit_record.object)
        {
            const float maxCos = std::sqrt(
                1.0f -
                light->radious() * light->radious() /
                    (hit_record.point - light->center()).square_length());
            const float  omega = 2.0f * (1.0f - maxCos);
            const vector normal =
                dot_product(light_hit_record.normal, ray.direction()) < 0.0f
                    ? light_hit_record.normal
                    : -light_hit_record.normal;

            result +=
                light_hit_record.material->emitted() *
                light_hit_record.material->albedo() *
                std::max(0.0f, dot_product(light_ray.direction(), normal)) *
                omega;
        }
    }

    if(result != color::rgb(0.0f, 0.0f, 0.0f))
    {
        spdlog::debug("probe_lights(): {}", result);
    }

    return result;
}

color trace(
    const ray& ray, const kernel_constants& constants, std::size_t depth_left)
{
    RT_PROFILE_FUNCTION();

    rt::hit_record hit_record;

    if(constants.scene.hit(
           ray, constants.min_hit_range, constants.max_hit_range, hit_record))
    {
        rt::ray     scattered;
        vector      attenuation;
        const auto* material = hit_record.material;
        const auto  emitted  = material->emitted();

        if(depth_left > 0 &&
           material->scatter(ray, hit_record, attenuation, scattered))
        {
            return material->emitted() +
                   probe_lights(ray, hit_record, constants) +
                   attenuation * trace(scattered, constants, depth_left - 1);
        }
        else
        {
            return material->emitted();
        }
    }
    else
    {
        vector unit_direction = ray.direction().normalized();

        const float t = 0.5f * (unit_direction.y + 1.0f);

        return ((1.0f - t) * color::rgb(1.0f, 1.0f, 1.0f) +
                t * color::rgb(0.5f, 0.7f, 1.0f)) *
               0.3f;
    }
}

void kernel(
    const float             x,
    const float             y,
    const kernel_constants& constants,
    color&                  pixel)
{
    RT_PROFILE_FUNCTION();
    RT_PROFILE_VALUE("u", x);
    RT_PROFILE_VALUE("v", y);

    rt::ray eye_to_pixel = constants.scene.camera.ray(x, y);

    pixel = trace(eye_to_pixel, constants, constants.iterations);
}
