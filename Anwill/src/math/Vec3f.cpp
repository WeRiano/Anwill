#include <cmath>

#include "math/Math.h"
#include "math/Vec3f.h"

namespace Anwill::Math {

    Vec3f::Vec3f()
        : X(0.0f), Y(0.0f), Z(0.0f)
    {}

    Vec3f::Vec3f(float x, float y, float z)
        : X(x), Y(y), Z(z)
    {}

    Vec3f& Vec3f::operator=(const Vec2f& other)
    {
        X = other.X;
        Y = other.Y;
        Z = 0.0f;
        return *this;
    }

    Vec3f Vec3f::operator+(const Vec3f& other) const
    {
        Vec3f res(X + other.X, Y + other.Y, Z + other.Z);
        return res;
    }

    Vec3f Vec3f::operator-(const Vec3f& other) const
    {
        return {X - other.X, Y - other.Y, Z - other.Z};
    }

    void Vec3f::operator+=(const Vec3f& other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
    }

    void Vec3f::operator-=(const Vec3f& other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
    }

    void Vec3f::operator*=(const float s)
    {
        X *= s;
        Y *= s;
        Z *= s;
    }

    Vec3f Vec3f::operator/(const float d) const
    {
        return {X / d, Y / d, Z / d};
    }

    bool Vec3f::operator>=(const Vec3f& other) const
    {
        return X >= other.X || Y >= other.Y || Z >= other.Z;
    }

    bool Vec3f::operator<=(const Vec3f& other) const
    {
        return X <= other.X || Y <= other.Y || Z <= other.Z;
    }

    bool Vec3f::operator>(const Vec3f& other) const
    {
        return X > other.X || Y > other.Y || Z > other.Z;
    }

    bool Vec3f::operator<(const Vec3f& other) const
    {
        return X < other.X || Y < other.Y || Z < other.Z;
    }

    Vec3f operator-(const Vec3f rhs)
    {
        return {-rhs.X, -rhs.Y, -rhs.Z};
    }

    Vec3f operator*(const Vec3f& lhs, const float rhs)
    {
        return {lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs};
    }

    Vec3f operator*(const float lhs, const Vec3f& rhs)
    {
        return {rhs.X * lhs, rhs.Y * lhs, rhs.Z * lhs};
    }

    Vec3f::operator Vec2f() const
    {
        return {X, Y};
    }

    Vec3f Vec3f::Abs() const
    {
        return {Math::Abs(X), Math::Abs(Y), Math::Abs(Z)};
    }

    Vec3f Vec3f::Negate() const
    {
        return {-X, -Y, -Z};
    }

    float Vec3f::Min() const
    {
        return Math::Min(Math::Min(X, Y), Z);
    }

    float Vec3f::Max() const
    {
        return Math::Max(Math::Max(X, Y), Z);
    }

    float Vec3f::DotProduct(const Vec3f& other) const
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    Vec3f Vec3f::CrossProduct(const Vec3f& other) const
    {
        return {Y * other.Z - Z * other.Y,
                Z * other.X - X * other.Z,
                X * other.Y - Y * other.X};
    }

    void Vec3f::Move(float deltaX, float deltaY, float deltaZ)
    {
        X += deltaX;
        Y += deltaY;
        Z += deltaZ;
    }

    void Vec3f::Normalize()
    {
        float length = GetLength();
        if (length == 0.0f)
        {
            return;
        }
        X = X / length;
        Y = Y / length;
        Z = Z / length;
    }

    bool Vec3f::IsZero() const
    {
        return X == 0.0f and Y == 0.0f and Z == 0.0f;
    }

    float Vec3f::GetLength() const
    {
        return sqrtf(X * X + Y * Y + Z * Z);
    }
}