#include <cmath>
#include <raytracer/debug/profile.hpp>
#include <raytracer/segment.hpp>

namespace rt
{

struct float_segment
{
    float begin, end;
};

std::optional<segment> overlap(const segment& lhs, const segment& rhs)
{
    ZoneScoped;

    const float begin = std::min(lhs.begin, rhs.begin);
    const float end   = std::max(lhs.end, rhs.end);

    if(begin < end)
    {
        return std::make_optional(segment{begin, end});
    }
    else
    {
        return std::nullopt;
    }
}
} // namespace rt
