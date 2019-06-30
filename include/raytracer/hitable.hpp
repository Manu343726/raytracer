#ifndef RAYTRACER_HITABLE_HPP
#define RAYTRACER_HITABLE_HPP

#include <raytracer/ray.hpp>
#include <raytracer/vector.hpp>

#include <memory>
#include <string>
#include <tinyrefl/api.hpp>

namespace rt
{

struct material;

struct hit_record
{
    float               t;
    rt::vector          point;
    rt::vector          normal;
    const rt::material* material;
};

struct hitable
{
    virtual ~hitable() = default;

    virtual bool
        hit(const rt::ray& ray,
            const float    min_t,
            const float    max_t,
            hit_record&    hit) const = 0;

    bool hit(const rt::ray& ray, hit_record& hit) const;

    virtual std::string to_string() const;
};

std::ostream& operator<<(std::ostream& os, const hitable& hitable);

template<typename Hitable, typename... Args>
std::unique_ptr<rt::hitable> make_hitable(Args&&... args)
{
    return {std::make_unique<Hitable>(std::forward<Args>(args)...)};
}

std::unique_ptr<rt::hitable>
    make_hitable(const std::string& type, const tinyrefl::json& args);

} // namespace rt

#endif // RAYTRACER_HITABLE_HPP
