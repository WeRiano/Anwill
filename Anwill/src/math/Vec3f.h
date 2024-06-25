#pragma once

#include "math/Vec2f.h"

namespace Anwill::Math {

    struct Vec3f
    {
    public:
        float X, Y, Z;

        Vec3f();
        Vec3f(float x, float y, float z);

        Vec3f& operator=(const Vec2f& other);
        Vec3f operator+(const Vec3f& other) const;
        Vec3f operator-(const Vec3f& other) const;
        void operator+=(const Vec3f& other);
        void operator-=(const Vec3f& other);
        void operator*=(float s);
        Vec3f operator/(float d) const;
        /**
         * Greater than and less than (or equals) operators return true if and only if the condition
         * is satisfied for all coordinates.
         */
        bool operator>=(const Vec3f& other) const;
        bool operator<=(const Vec3f& other) const;
        bool operator>(const Vec3f& other) const;
        bool operator<(const Vec3f& other) const;
        operator Vec2f() const;

        Vec3f Abs() const;
        Vec3f Negate() const;
        float DotProduct(const Vec3f& other) const;
        Vec3f CrossProduct(const Vec3f& other) const;
        void Move(float deltaX, float deltaY, float deltaZ);
        void Normalize();
        bool IsZero() const;
        float GetLength() const;
    };

    Vec3f operator-(const Vec3f rhs);
    Vec3f operator*(const Vec3f& lhs, const float rhs);
    Vec3f operator*(const float lhs, const Vec3f& rhs);
}