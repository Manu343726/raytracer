#include <raytracer/material.hpp>

using namespace rt;

color material::emitted() const
{
    return color::rgb(0.0f, 0.0f, 0.0f);
}

color material::albedo() const
{
    return color::rgb(1.0f, 1.0f, 1.0f);
}

std::string material::to_string() const
{
    return "";
}
