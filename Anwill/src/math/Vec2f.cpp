#include <cmath>

#include "math/Math.h"
#include "math/Vec2f.h"
#include "math/Vec3f.h"

namespace Anwill::Math {

    Vec2f::Vec2f()
        : X(0.0f), Y(0.0f)
    {}

    Vec2f::Vec2f(float x, float y)
        : X(x), Y(y)
    {}

    Vec2f& Vec2f::operator=(const Vec3f& other)
    {
        X = other.X;
        Y = other.Y;
        return *this;
    }

    Vec2f Vec2f::operator+(const Vec2f& other) const
    {
        return {X + other.X, Y + other.Y};
    }

    Vec2f Vec2f::operator-(const Vec2f& other) const
    {
        return {X - other.X, Y - other.Y};
    }

    void Vec2f::operator+=(const Vec2f& other)
    {
        X += other.X;
        Y += other.Y;
    }

    void Vec2f::operator-=(const Vec2f& other)
    {
        X -= other.X;
        Y -= other.Y;
    }

    void Vec2f::operator*=(const float other)
    {
        X *= other;
        Y *= other;
    }

    Vec2f operator-(const Vec2f rhs)
    {
        return {rhs.X, rhs.Y};
    }

    Vec2f operator*(const Vec2f& lhs, const float rhs)
    {
        return {lhs.X * rhs, lhs.Y * rhs};
    }

    Vec2f Math::operator*(const float lhs, const Vec2f& rhs)
    {
        return {lhs * rhs.X, lhs * rhs.Y};
    }

    bool Vec2f::operator>=(const Vec2f& other) const
    {
        return X >= other.X || Y >= other.Y;
    }

    bool Vec2f::operator<=(const Vec2f& other) const
    {
        return X <= other.X || Y <= other.Y;
    }

    bool Vec2f::operator>(const Vec2f& other) const
    {
        return X > other.X || Y > other.Y;
    }

    bool Vec2f::operator<(const Vec2f& other) const
    {
        return X < other.X || Y < other.Y;
    }

    Vec2f::operator Vec3f() const
    {
        return {X, Y, 0.0f};
    }

    Vec2f Vec2f::Abs() const
    {
        return {Math::Abs(X), Math::Abs(Y)};
    }

    Vec2f Vec2f::Negate() const
    {
        return {-X, -Y};
    }

    Vec2f Vec2f::NegateX() const
    {
        return {-X, Y};
    }

    Vec2f Vec2f::NegateY() const
    {
        return {X, -Y};
    }

    float Vec2f::Min() const
    {
        return Math::Min(X, Y);
    }

    float Vec2f::Max() const
    {
        return Math::Max(X, Y);
    }

    float Vec2f::ScalarProjection(Vec2f vec2f) const
    {
        vec2f.Normalize();
        return DotProduct(vec2f);
    }

    float Vec2f::DotProduct(const Vec2f& other) const
    {
        return X * other.X + Y * other.Y;
    }

    void Vec2f::Move(float deltaX, float deltaY)
    {
        X += deltaX;
        Y += deltaY;
    }

    void Vec2f::Normalize()
    {
        float length = GetLength();
        X = X / length;
        Y = Y / length;
    }

    void Vec2f::Clamp(const Vec2f& min, const Vec2f& max)
    {
        if(X < min.X) { X = min.X; }
        if(X > max.X) { X = max.X; }
        if(Y < min.Y) { Y = min.Y; }
        if(Y > max.Y) { Y = max.Y; }
    }

    float Vec2f::GetLength() const
    {
        return sqrtf(X * X + Y * Y);
    }

    std::string Vec2f::ToString() const
    {
        return std::string("[" + std::to_string(X) + ", " + std::to_string(Y) + "]");
    }
}