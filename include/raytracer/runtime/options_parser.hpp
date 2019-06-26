#ifndef RAYTRACER_RUNTIME_OPTIONSPARSER_HPP_INCLUDED
#define RAYTRACER_RUNTIME_OPTIONSPARSER_HPP_INCLUDED

#include <cxxopts.hpp>
#include <tinyrefl/api.hpp>

#include <ostream>
#include <string>
#include <type_traits>

namespace rt
{

namespace runtime
{

namespace detail
{

template<typename MemberVariable>
void add_option(MemberVariable, cxxopts::Options& options);

template<typename Settings>
void add_options(cxxopts::Options& options);

template<typename MemberVariable>
constexpr auto option_name(MemberVariable var);

template<typename MemberVariable>
constexpr auto option_short_name(MemberVariable var);

template<typename MemberVariable>
constexpr auto option_description(MemberVariable var);

template<typename Settings, typename MemberVariable>
void apply_option(
    const cxxopts::ParseResult& options, Settings& settings, MemberVariable);

} // namespace detail

template<typename Settings>
cxxopts::Options
    generate_options(const char* exec_name, const char* exec_description)
{
    cxxopts::Options options{exec_name, exec_description};

    detail::add_options<Settings>(options);

    return options;
}

template<typename Settings>
void apply_options(const cxxopts::ParseResult& options, Settings& settings)
{
    tinyrefl::visit<Settings>(
        tinyrefl::member_variable_visitor([&](const auto& var) {
            detail::apply_option(options, settings, var);
        }));
}

namespace detail
{

template<typename T, typename = void>
struct is_istream_readable : std::false_type
{
};

template<typename T>
struct is_istream_readable<
    T,
    std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
    : std::true_type
{
};

static_assert(is_istream_readable<int>::value);

template<typename T>
void check()
{
    static_assert(is_istream_readable<T>());
}

template<typename MemberPtr>
struct result_type;

template<typename T, typename Class>
struct result_type<T Class::*>
{
    using type = T;
};

template<typename MemberVariable>
void add_option(MemberVariable, cxxopts::Options& options)
{
    constexpr MemberVariable var;
    using value_type = typename MemberVariable::value_type;

    const auto name        = option_name(var);
    const auto short_name  = option_short_name(var);
    const auto description = option_description(var);

    if constexpr(
        std::is_class_v<value_type> && tinyrefl::has_metadata<value_type>())
    {
        add_options<value_type>(options);
    }
    else if constexpr(is_istream_readable<value_type>())
    {
        options.add_option(
            "",
            short_name.str(),
            name.str(),
            description.str(),
            cxxopts::value<value_type>(),
            "");
    }
}

template<typename Settings>
void add_options(cxxopts::Options& options)
{
    tinyrefl::visit<Settings>(tinyrefl::member_variable_visitor(
        [&](const auto& var) { add_option(var, options); }));
} // namespace detail

template<typename Entity>
constexpr tinyrefl::entities::attribute*
    find_attribute(Entity, const tinyrefl::string& name)
{
    const auto* ptr = tinyrefl::find(Entity().attributes(), name);

    if(ptr == Entity().attribuites().end())
    {
        return nullptr;
    }
    else
    {
        return ptr;
    }
}

template<typename MemberVariable>
constexpr auto option_name(MemberVariable)
{
    constexpr MemberVariable var;

    if(var.has_attribute("rt::name") &&
       var.attribute("rt::name").arguments().size() == 1)
    {
        return var.attribute("rt::name").arguments()[0].pad(1, 1);
    }

    return var.name();
}

template<typename MemberVariable>
constexpr auto option_short_name(MemberVariable)
{
    constexpr MemberVariable var;

    if(var.has_attribute("rt::short_name") &&
       var.attribute("rt::short_name").arguments().size() == 1)
    {
        return var.attribute("rt::short_name").arguments()[0].pad(1, 1);
    }
    else
    {
        return tinyrefl::string{""};
    }
}

template<typename MemberVariable>
constexpr auto option_description(MemberVariable)
{
    constexpr MemberVariable var;

    if(var.has_attribute("rt::description") &&
       var.attribute("rt::description").arguments().size() == 1)
    {
        return var.attribute("rt::description").arguments()[0].pad(1, 1);
    }
    else
    {
        return ctti::detail::cstring{""};
    }
}

template<typename T, typename = void>
struct is_copy_constructible : public std::is_copy_constructible<T>
{
};

template<typename T>
struct is_copy_constructible<
    T,
    std::void_t<decltype(
        std::declval<T>().begin(), std::declval<typename T::value_type>())>>
    : std::is_copy_constructible<typename T::value_type>
{
};

template<typename Settings, typename MemberVariable>
void apply_option(
    const cxxopts::ParseResult& options, Settings& settings, MemberVariable)
{
    constexpr MemberVariable var;
    const std::string        name = option_name(var).str();
    using value_type              = typename MemberVariable::value_type;

    if constexpr(
        std::is_class_v<value_type> && tinyrefl::has_metadata<value_type>())
    {
        apply_options(options, var.get(settings));
    }
    else
    {
        if constexpr(is_copy_constructible<value_type>::value)
        {
            if(options.count(name))
            {
                var.get(settings) = options[name].as<value_type>();
            }
        }
    }
}

} // namespace detail
} // namespace runtime

} // namespace rt

#endif // RAYTRACER_RUNTIME_OPTIONSPARSER_HPP_INCLUDED
