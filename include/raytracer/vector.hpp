#ifndef RAYTRACER_VECTOR_HPP_INCLUDED
#define RAYTRACER_VECTOR_HPP_INCLUDED

#include <tinyrefl/api.hpp>

namespace rt
{

struct vector
{
    union {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float r, g, b;
        };
        struct
        {
            float h, s, v;
        };
        float coords[3];
    };

    enum axis
    {
        X,
        Y,
        Z
    };

    enum channel
    {
        R = X,
        G = Y,
        B = Z
    };

    using color = vector;

    float operator[](const int i) const;
    float operator[](const axis axis) const;
    float operator[](const channel channel) const;

    float& operator[](const int i);
    float& operator[](const axis axis);
    float& operator[](const channel channel);

    vector& operator+=(const vector& other);
    vector& operator-=(const vector& other);
    vector& operator*=(const float value);
    vector& operator/=(const float value);

    float  length() const;
    float  square_length() const;
    bool   is_normalized() const;
    vector normalized() const;

    static vector x_axis();
    static vector y_axis();
    static vector z_axis();

    static vector spheric_random();
    static vector hemispheric_random(const vector& up);

    static color rgb(const float r, const float g, const float b);
    static color hsv(const float h, const float s, const float v);
    static color hsv(const float value);
    static color random_rgb();
};

void from_json(const tinyrefl::json& json, vector& v);

using color = vector;

std::ostream& operator<<(std::ostream& os, const vector& vector);

bool operator==(const vector& lhs, const vector& rhs);
bool operator!=(const vector& lhs, const vector& rhs);

vector operator+(const vector& lhs, const vector& rhs);
vector operator-(const vector& lhs, const vector& rhs);
vector operator*(const vector& lhs, const vector& rhs);
float  dot_product(const vector& lhs, const vector& rhs);
vector operator*(const float lhs, const vector& rhs);
vector operator*(const vector& lhs, const float rhs);
vector operator/(const vector& lhs, const float rhs);
vector
      clamp(vector v, const vector::axis axis, const float min, const float max);
color clamp(
    color v, const color::channel channel, const float min, const float max);
vector clamp(const vector& v, const vector& min, const vector& max);
vector reflect(const vector& v, const vector& axis);

} // namespace rt

#endif // RAYTRACER_VECTOR_HPP_INCLUDED
