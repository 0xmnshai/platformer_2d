#pragma once

#include <iostream>
#include <cmath>

class Vector2f
{
public:
    float x;
    float y;

    Vector2f() : x(0), y(0) {};
    Vector2f(float x, float y) : x(x), y(y) {};

    Vector2f operator+(const Vector2f &other) const;
    Vector2f operator-(const Vector2f &other) const;
    Vector2f operator*(float scalar) const;
    Vector2f &operator+=(const Vector2f &other);

    float length() const;
    void print() const;
};

std::ostream &operator<<(std::ostream &os, const Vector2f &vec);