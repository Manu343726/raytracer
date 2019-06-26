#ifndef RAYTRACER_REFLECTION_FACTORY_HPP_INCLUDED
#define RAYTRACER_REFLECTION_FACTORY_HPP_INCLUDED

#include <tinyrefl/api.hpp>

#include <cassert>
#include <memory>

namespace rt
{

namespace reflection
{

using factory_instance_ptr = std::shared_ptr<void>;

factory_instance_ptr
    factory(const std::string& type, const tinyrefl::json& args);

template<typename Interface>
std::unique_ptr<Interface>
    factory(const std::string& type, const tinyrefl::json& args)
{
    factory_instance_ptr instance     = factory(type, args);
    void*                raw_instance = instance.get();
    assert(instance.use_count() == 1);
    instance.reset();
    assert(instance.use_count() == 0);

    return std::unique_ptr<Interface>{
        reinterpret_cast<Interface*>(raw_instance)};
}
} // namespace reflection

} // namespace rt

#endif // RAYTRACER_REFLECTION_FACTORY_HPP_INCLUDED
