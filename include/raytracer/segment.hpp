#ifndef RAYTRACER_SEGMENT_HPP_INCLUDED
#define RAYTRACER_SEGMENT_HPP_INCLUDED

#include <optional>

namespace rt
{

struct segment
{
    float begin, end;
};

std::optional<segment> overlap(const segment& lhs, const segment& rhs);

} // namespace rt

#endif // RAYTRACER_SEGMENT_HPP_INCLUDED
