#include "vector2f.hpp"

Vector2f Vector2f::operator+(const Vector2f &other) const
{
    return Vector2f(x + other.x, y + other.y);
}

Vector2f Vector2f::operator-(const Vector2f &other) const
{
    return Vector2f(x - other.x, y - other.y);
}

Vector2f Vector2f::operator*(float scalar) const
{
    return Vector2f(x * scalar, y * scalar);
}

Vector2f &Vector2f::operator+=(const Vector2f &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

float Vector2f::length() const
{
    return std::sqrt(x * x + y * y);
}

void Vector2f::print() const
{
    std::cout << "Vector2f(" << x << ", " << y << ")\n";
}

std::ostream &operator<<(std::ostream &os, const Vector2f &vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}