#include <range/v3/all.hpp>
#include <raytracer/debug/profile.hpp>
#include <raytracer/hitables/kdtree.hpp>

using namespace rt;
using namespace rt::hitables;
using iterator = kdtree::iterator;

kdtree::kdtree(iterator objects_begin, iterator objects_end)
    : _boundingBox{ranges::accumulate(
          ranges::subrange(objects_begin, objects_end) |
              ranges::views::indirect |
              ranges::views::transform(&hitable::bounding_box),
          box::nil(),
          surrounding_box)},
      _objects_begin{objects_begin},
      _objects_end{objects_end}
{
    ZoneScoped;

    const auto axis = vector::random_axis();

    std::sort(
        objects_begin, objects_end, [axis](const auto& lhs, const auto& rhs) {
            const auto& lhs_box = lhs->bounding_box();
            const auto& rhs_box = rhs->bounding_box();

            return lhs_box.min()[axis] - rhs_box.min()[axis] < 0.0f;
        });

    const std::size_t total_objects = std::distance(objects_begin, objects_end);

    if(total_objects > 2)
    {
        auto pivot = objects_begin + total_objects / 2;

        _leftChild  = std::make_unique<kdtree>(objects_begin, pivot);
        _rightChild = std::make_unique<kdtree>(pivot, objects_end);
    }
}

box kdtree::bounding_box() const
{
    return _boundingBox;
}

bool kdtree::is_leaf() const
{
    return _leftChild == nullptr;
}

bool kdtree::hit(
    const rt::ray&  ray,
    const float     min_t,
    const float     max_t,
    rt::hit_record& hit_record) const
{
    ZoneNamedN(KdTreeHit, "kdtree::hit", true);

    if(!_boundingBox.contains(ray, min_t, max_t))
    {
        return false;
    }

    if(is_leaf())
    {
        return std::any_of(
            _objects_begin, _objects_end, [&](const auto& hitable) {
                return hitable->hit(ray, min_t, max_t, hit_record);
            });
    }
    else
    {
        rt::hit_record left_record;
        rt::hit_record right_record;

        bool left_hit  = _leftChild->hit(ray, min_t, max_t, left_record);
        bool right_hit = _rightChild->hit(ray, min_t, max_t, right_record);

        if(left_hit && right_hit)
        {
            if(left_record.t < right_record.t)
            {
                hit_record = left_record;
            }
            else
            {
                hit_record = right_record;
            }

            return true;
        }
        else if(left_hit)
        {
            hit_record = left_record;
            return true;
        }
        else if(right_hit)
        {
            hit_record = right_record;
            return true;
        }
        else
        {
            return false;
        }
    }
}
