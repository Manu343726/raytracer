#ifndef RAYTRACER_HITABLES_KDTREE_HPP_INCLUDED
#define RAYTRACER_HITABLES_KDTREE_HPP_INCLUDED

#include <raytracer/hitable.hpp>

namespace rt
{

namespace hitables
{

struct kdtree final : public rt::hitable
{
    using iterator = std::vector<std::unique_ptr<rt::hitable>>::iterator;

    kdtree(iterator objects_begin, iterator objects_end);

    rt::box bounding_box() const override;
    bool
        hit(const rt::ray&  ray,
            const float     min_t,
            const float     max_t,
            rt::hit_record& hit_record) const override;

private:
    rt::box                 _boundingBox;
    std::unique_ptr<kdtree> _leftChild, _rightChild;
    iterator                _objects_begin, _objects_end;

    bool is_leaf() const;
};
} // namespace hitables

} // namespace rt

#endif // RAYTRACER_HITABLES_KDTREE_HPP_INCLUDED
