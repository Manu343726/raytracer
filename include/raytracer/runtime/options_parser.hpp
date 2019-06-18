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
    tinyrefl::visit_class<Settings>(
        [&](const auto& name,
            const std::size_t /* depth */,
            const auto& var,
            TINYREFL_STATIC_VALUE(tinyrefl::entity::MEMBER_VARIABLE)) {
            detail::apply_option(options, settings, var);
        });
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
    using value_type =
        typename result_type<typename MemberVariable::value_type>::type;

    constexpr auto name        = option_name(var);
    constexpr auto short_name  = option_short_name(var);
    constexpr auto description = option_description(var);

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
    tinyrefl::visit_class<Settings>(
        [&](const auto& name,
            const std::size_t /* depth */,
            const auto& var,
            TINYREFL_STATIC_VALUE(tinyrefl::entity::MEMBER_VARIABLE)) {
            add_option(var, options);
        });
}

template<typename MemberVariable>
constexpr auto option_name(MemberVariable var)
{
    if constexpr(
        var.has_attribute("name") &&
        var.get_attribute("name").namespace_.full_name() == "rt" &&
        var.get_attribute("name").args.size() == 1)
    {
        return var.get_attribute("name").args[0].pad(1, 1);
    }
    else
    {
        return MemberVariable::name.name();
    }
}

template<typename MemberVariable>
constexpr auto option_short_name(MemberVariable var)
{
    if constexpr(
        var.has_attribute("short_name") &&
        var.get_attribute("short_name").namespace_.full_name() == "rt" &&
        var.get_attribute("short_name").args.size() == 1)
    {
        return var.get_attribute("short_name").args[0].pad(1, 1);
    }
    else
    {
        return ctti::detail::cstring{""};
    }
}

template<typename MemberVariable>
constexpr auto option_description(MemberVariable var)
{
    if constexpr(
        var.has_attribute("description") &&
        var.get_attribute("description").namespace_.full_name() == "rt" &&
        var.get_attribute("description").args.size() == 1)
    {
        return var.get_attribute("description").args[0].pad(1, 1);
    }
    else
    {
        return ctti::detail::cstring{""};
    }
}

template<typename Settings, typename MemberVariable>
void apply_option(
    const cxxopts::ParseResult& options, Settings& settings, MemberVariable)
{
    constexpr MemberVariable var;
    const std::string        name = option_name(var).str();
    using value_type =
        typename result_type<typename MemberVariable::value_type>::type;

    if constexpr(
        std::is_class_v<value_type> && tinyrefl::has_metadata<value_type>())
    {
        apply_options(options, var.get(settings));
    }
    else
    {

        if(options.count(name))
        {
            var.get(settings) = options[name].as<value_type>();
        }
    }
}

} // namespace detail
} // namespace runtime

} // namespace rt

#endif // RAYTRACER_RUNTIME_OPTIONSPARSER_HPP_INCLUDED
