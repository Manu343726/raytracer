#include <raytracer/vector.hpp>
#include <raytracer/math.hpp>
#include <cassert>
#include <cmath>

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

float vector::length() const
{
    return std::sqrt(square_length());
}

float vector::square_length() const
{
    return x*x + y*y + z*z;
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

color color::random_rgb()
{
    return rgb(
        rt::random(0.0f, 1.0f),
        rt::random(0.0f, 1.0f),
        rt::random(0.0f, 1.0f)
    );
}

bool operator==(const vector& lhs, const vector& rhs)
{
    return rt::float_equal(lhs.x, rhs.x) &&
           rt::float_equal(lhs.y, rhs.y) &&
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

float  operator*(const vector& lhs, const vector& rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

vector operator*(const float lhs,   const vector& rhs)
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

vector clamp(vector vector, const vector::axis axis, const float min, const float max)
{
    vector[axis] = rt::clamp(vector[axis], min, max);

    return vector;
}

vector clamp(vector vector, const vector::channel channel, const float min, const float max)
{
    return rt::clamp(std::move(vector), static_cast<vector::axis>(channel), min, max);
}

vector clamp(const vector& v, const vector& min, const vector& max)
{
    return {
        rt::clamp(v.x, min.x, max.x),
        rt::clamp(v.y, min.y, max.y),
        rt::clamp(v.z, min.z, max.z)
    };
}

vector reflect(const vector& v, const vector& axis)
{
    assert(axis.is_normalized() && "Cannot reflect on non-normalized axis vector");

    return 2.0f * (axis * v) * axis - v;
}

}
