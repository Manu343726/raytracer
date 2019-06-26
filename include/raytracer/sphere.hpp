#ifndef RAYTRACER_SPHERE_HPP_INCLUDED
#define RAYTRACER_SPHERE_HPP_INCLUDED

#include <raytracer/hitable.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

#include <optional>

namespace rt
{

class sphere : public rt::hitable
{
public:
    sphere(const rt::vector& center, const float radious);

    const rt::vector& center() const;
    float             radious() const;

    std::optional<float>      hit(const rt::ray& ray) const;
    std::optional<rt::vector> hit_point(const rt::ray& ray) const;

    bool
        hit(const rt::ray&  ray,
            const float     min_t,
            const float     max_t,
            rt::hit_record& hit) const override;

    std::string to_string() const override;

private:
    rt::vector _center;
    float      _radious;
};

} // namespace rt

#endif // RAYTRACER_SPHERE_HPP_INCLUDED
