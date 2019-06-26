#ifndef RAYTRACER_REFLECTION_FROMJSON_HPP_INCLUDED
#define RAYTRACER_REFLECTION_FROMJSON_HPP_INCLUDED

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include <tinyrefl/api.hpp>

namespace rt
{

namespace reflection
{

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

                ctor_arg =
                    tinyrefl::from_json<std::decay_t<decltype(ctor_arg)>>(
                        it.value());
            });

            if(overload_matches)
            {
                std::apply(
                    [&result](auto&&... args) {
                        result = std::make_unique<T>(
                            std::forward<decltype(args)>(args)...);
                    },
                    ctor_args);
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
