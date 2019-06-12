#ifndef RAYTRACER_UTILS_HPP_INCLUDED
#define RAYTRACER_UTILS_HPP_INCLUDED

#include <utility>

namespace ray
{

namespace utils
{

template<typename T, typename... Args>
void construct(void* where, Args&&... args)
{
    new(where) T{std::forward<Args>(args)...};
}

template<typename T>
void destroy(T* object)
{
    object->~T();
}

template<typename T, typename U>
void destroy(U* object)
{
    destroy(reinterpret_cast<T*>(object));
}

template<typename T>
void destroy(T& object)
{
    destroy(&object);
}

}

}

#endif // RAYTRACER_UTILS_HPP_INCLUDED
