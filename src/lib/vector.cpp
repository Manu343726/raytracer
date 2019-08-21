
#include <cassert>
#include <cmath>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <raytracer/math.hpp>
#include <raytracer/vector.hpp>

namespace rt
{

float vector::operator[](const int i) const
{
    return coords[i];
}

float vector::operator[](const axis axis) const
{
    return coords[axis];
}

float vector::operator[](const channel channel) const
{
    return coords[channel];
}

float& vector::operator[](const int i)
{
    return coords[i];
}

float& vector::operator[](const axis axis)
{
    return coords[axis];
}

float& vector::operator[](const channel channel)
{
    return coords[channel];
}

vector& vector::operator+=(const vector& other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

vector& vector::operator-=(const vector& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

vector& vector::operator*=(const float value)
{
    x *= value;
    y *= value;
    z *= value;

    return *this;
}

vector& vector::operator/=(const float value)
{
    x /= value;
    y /= value;
    z /= value;

    return *this;
}

vector vector::operator-() const
{
    return {-x, -y, -z};
}

float vector::length() const
{
    return std::sqrt(square_length());
}

float vector::square_length() const
{
    return x * x + y * y + z * z;
}

vector vector::normalized() const
{
    return *this / length();
}

bool vector::is_normalized() const
{
    return rt::float_equal(square_length(), 1.0f);
}

color color::rgb(const float r, const float g, const float b)
{
    return {r, g, b};
}

color color::hsv(const float h, const float s, const float v)
{
    if(s == 0.f)
    {
        return {v, v, v};
    }

    color rgb = {};

    const float k = h * 6.f;
    const int   d = int(std::floor(k));
    const float C = v * s;
    const float X = C * (1.f - std::fabs(std::fmod(k, 2.f) - 1.f));
    const float m = v - C;

    switch(d)
    {
    case 0:
        rgb = {C, X, 0.f};
        break;
    case 1:
        rgb = {X, C, 0.f};
        break;
    case 2:
        rgb = {0.f, C, X};
        break;
    case 3:
        rgb = {0.f, X, C};
        break;
    case 4:
        rgb = {X, 0.f, C};
        break;
    default:
        rgb = {C, 0.f, X};
        break;
    }

    rgb += color{m, m, m};
    return rgb;
}

color color::hsv(const float h)
{
    return hsv(h, 1.0f, 1.0f);
}

color color::random_rgb()
{
    return rgb(
        rt::random(0.0f, 1.0f), rt::random(0.0f, 1.0f), rt::random(0.0f, 1.0f));
}

std::uint32_t linearToSRGB(const float channel)
{
    float x = std::max(0.0f, channel);

    x = std::max(1.055f * std::pow(x, 0.416666667f) - 0.055f, 0.0f);

    return std::min(static_cast<std::uint32_t>(x * 255.9f), 255u);
}

vector vector::spheric_random()
{
    vector v;

    do
    {
        v = 2.0f * vector{rt::random(), rt::random(), rt::random()} -
            vector{1.0f, 1.0f, 1.0f};
    } while(v.square_length() >= 1.0f);

    return v;
}

vector vector::hemispheric_random(const vector& v)
{
    return (v + spheric_random()).normalized();
}

void from_json(const tinyrefl::json& json, vector& vector)
{
    tinyrefl::json::const_iterator it;

#define FROM_JSON_TRY_ASSIGN(key)                           \
    bool TINYREFL_PP_CAT(contains_field_, key) = false;     \
    it = json.find(TINYREFL_PP_STR(key));                   \
                                                            \
    if(it != json.end())                                    \
    {                                                       \
        vector.key                            = it.value(); \
        TINYREFL_PP_CAT(contains_field_, key) = true;       \
    }

    FROM_JSON_TRY_ASSIGN(x)
    FROM_JSON_TRY_ASSIGN(y)
    FROM_JSON_TRY_ASSIGN(z)

    FROM_JSON_TRY_ASSIGN(r)
    FROM_JSON_TRY_ASSIGN(g)
    FROM_JSON_TRY_ASSIGN(b)

    FROM_JSON_TRY_ASSIGN(h)
    FROM_JSON_TRY_ASSIGN(s)
    FROM_JSON_TRY_ASSIGN(v)

#undef FROM_JSON_TRY_ASSIGN

    if(contains_field_h && contains_field_s && contains_field_v)
    {
        vector = color::hsv(vector.h, vector.s, vector.v);
    }
}

std::ostream& operator<<(std::ostream& os, const vector& vector)
{
    fmt::print(os, "({}, {}, {})", vector.x, vector.y, vector.z);
    return os;
}

bool operator==(const vector& lhs, const vector& rhs)
{
    return rt::float_equal(lhs.x, rhs.x) && rt::float_equal(lhs.y, rhs.y) &&
           rt::float_equal(lhs.z, rhs.z);
}

bool operator!=(const vector& lhs, const vector& rhs)
{
    return !(lhs == rhs);
}

vector operator+(const vector& lhs, const vector& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

vector operator-(const vector& lhs, const vector& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

vector operator*(const vector& lhs, const vector& rhs)
{
    return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}

float dot_product(const vector& lhs, const vector& rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

vector cross_product(const vector& lhs, const vector& rhs)
{
    return {
          lhs.y * rhs.z - lhs.z * rhs.y,
        -(lhs.x * rhs.z - lhs.z * rhs.x),
          lhs.x * rhs.y - lhs.y * rhs.x
    };
}

vector operator*(const float lhs, const vector& rhs)
{
    return {rhs.x * lhs, rhs.y * lhs, rhs.z * lhs};
}

vector operator*(const vector& lhs, const float rhs)
{
    return rhs * lhs;
}

vector operator/(const vector& lhs, const float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

vector clamp(
    vector vector, const vector::axis axis, const float min, const float max)
{
    vector[axis] = rt::clamp(vector[axis], min, max);

    return vector;
}

vector clamp(
    vector                vector,
    const vector::channel channel,
    const float           min,
    const float           max)
{
    return rt::clamp(
        std::move(vector), static_cast<vector::axis>(channel), min, max);
}

vector clamp(const vector& v, const vector& min, const vector& max)
{
    return {rt::clamp(v.x, min.x, max.x),
            rt::clamp(v.y, min.y, max.y),
            rt::clamp(v.z, min.z, max.z)};
}

vector reflect(const vector& v, const vector& axis)
{
    return v - 2.0f * dot_product(v, axis) * axis;
}

std::optional<vector>
    refract(const vector& v, const vector& normal, const float refraction_index)
{
    const vector nv = v.normalized();
    const float  dt = dot_product(nv, normal);

    const float discriminant =
        1.0f - refraction_index * refraction_index * (1.0f - dt * dt);

    if(discriminant > 0.0f)
    {
        return std::make_optional(
            refraction_index * (nv - normal * dt) -
            normal * std::sqrt(discriminant));
    }
    else
    {
        return std::nullopt;
    }
}

} // namespace rt
