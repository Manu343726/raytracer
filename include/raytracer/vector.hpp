#ifndef RAYTRACER_VECTOR_HPP_INCLUDED
#define RAYTRACER_VECTOR_HPP_INCLUDED

namespace ray
{

struct vector
{
    union
    {
        struct
        {
            float x,y,z;
        };
        struct
        {
            float r,g,b;
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
        R=X,
        G=Y,
        B=Z
    };

    using color = vector;

    float operator[](const int i) const;
    float operator[](const axis axis) const;
    float operator[](const channel channel) const;

    float& operator[](const int i);
    float& operator[](const axis axis);
    float& operator[](const channel channel);

    float length() const;
    float square_length() const;
    bool is_normalized() const;
    vector normalized() const;

    static vector x_axis();
    static vector y_axis();
    static vector z_axis();

    static vector spheric_random();
    static vector hemispheric_random(const vector& up);

    static color rgb(const float r, const float g, const float b);
    static color hsv(const float h, const float s, const float v);
};

using color = vector;

vector operator+(const vector& lhs, const vector& rhs);
vector operator-(const vector& lhs, const vector& rhs);
float  operator*(const vector& lhs, const vector& rhs);
vector operator*(const float lhs,   const vector& rhs);
vector operator*(const vector& lhs, const float rhs);
vector operator/(const vector& lhs, const float rhs);
vector clamp(vector v, const vector::axis axis,       const float min, const float max);
vector clamp(vector v, const vector::channel channel, const float min, const float max);
vector clamp(const vector& v, const vector& min, const vector& max);
vector reflect(const vector& v, const vector& axis);

} // namespace ray

#endif // RAYTRACER_VECTOR_HPP_INCLUDED
