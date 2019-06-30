#ifndef RAYTRACER_REFLECTION_FROMJSON_HPP_INCLUDED
#define RAYTRACER_REFLECTION_FROMJSON_HPP_INCLUDED

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include <tinyrefl/api.hpp>

namespace rt
{

namespace reflection
{

template<typename Interface>
std::unique_ptr<Interface>
    factory(const std::string& type, const tinyrefl::json& args);

namespace
{

template<typename T>
struct is_unique_ptr : std::false_type
{
};

template<typename T, typename Deleter>
struct is_unique_ptr<std::unique_ptr<T, Deleter>> : std::true_type
{
};

template<typename T, typename = void>
struct element_type_extractor
{
    using type = T;
};

template<typename T>
struct element_type_extractor<T, std::void_t<typename T::element_type>>
{
    using type = typename T::element_type;
};

template<typename T>
using element_type = typename element_type_extractor<T>::type;

} // namespace

template<typename T>
std::unique_ptr<T> from_json(const tinyrefl::json& json)
{
    std::unique_ptr<T> result;

    constexpr auto class_ = tinyrefl::metadata<T>();

    tinyrefl::visit(
        class_, tinyrefl::constructor_visitor([&](const auto& ctor) {
            if(result != nullptr || ctor.arguments().size() != json.size())
            {
                return;
            }

            using ctor_signature =
                typename std::decay_t<decltype(ctor)>::Signature;

            using ctor_args_t = tinyrefl::meta::functor_t<
                std::tuple<>,
                tinyrefl::meta::
                    fmap_t<tinyrefl::meta::defer<std::decay>, ctor_signature>>;

            bool        overload_matches = true;
            ctor_args_t ctor_args;
            std::size_t index = 0;

            tinyrefl::meta::foreach(ctor_args, [&](auto& ctor_arg) {
                if(!overload_matches)
                {
                    return;
                }

                const auto ctor_arg_name = ctor.arguments()[index++].str();

                auto it = json.find(ctor_arg_name);

                if(it == json.end())
                {
                    overload_matches = false;
                    return;
                }

                using ctor_arg_type = std::decay_t<decltype(ctor_arg)>;

                if constexpr(
                    is_unique_ptr<ctor_arg_type>() &&
                    tinyrefl::has_metadata<element_type<ctor_arg_type>>())
                {
                    ctor_arg =
                        rt::reflection::factory<element_type<ctor_arg_type>>(
                            it.value()["type"], it.value()["args"]);
                }
                else
                {
                    static_assert(!is_unique_ptr<ctor_arg_type>());
                    ctor_arg = tinyrefl::from_json<ctor_arg_type>(it.value());
                }
            });

            if(overload_matches)
            {
                std::apply(
                    [&result](auto&&... args) {
                        result = std::make_unique<T>(
                            std::forward<decltype(args)>(args)...);
                    },
                    std::move(ctor_args));
            }
        }));

    if(result == nullptr)
    {
        spdlog::error(
            "Could not construct instance of {} from with arguments {} (Missing ctor overload?)",
            class_.full_name(),
            json.dump());
    }

    return result;
}

} // namespace reflection
} // namespace rt

#endif // RAYTRACER_REFLECTION_FROMJSON_HPP_INCLUDED
