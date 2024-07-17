#pragma once

#include <string>

namespace Anwill::Math {

    struct Vec3f;

    struct Vec2f
    {
    public:
        float X, Y;

        Vec2f();
        Vec2f(float x, float y);

        Vec2f& operator=(const Vec3f& other);
        Vec2f operator+(const Vec2f& other) const;
        Vec2f operator-(const Vec2f& other) const;
        void operator+=(const Vec2f& other);
        void operator-=(const Vec2f& other);
        void operator*=(float other);
        /**
         * Greater than and less than (or equals) operators return true if and only if the condition
         * is satisfied for all coordinates.
         */
        bool operator>=(const Vec2f& other) const;
        bool operator<=(const Vec2f& other) const;
        bool operator>(const Vec2f& other) const;
        bool operator<(const Vec2f& other) const;
        operator Vec3f() const;

        Vec2f Abs() const;
        Vec2f Negate() const;
        Vec2f NegateX() const;
        Vec2f NegateY() const;
        float Min() const;
        float Max() const;
        float ScalarProjection(Vec2f vec2f) const;
        float DotProduct(const Vec2f& other) const;
        void Move(float deltaX, float deltaY);
        void Normalize();
        void Clamp(const Vec2f& min, const Vec2f& max);
        float GetLength() const;
        std::string ToString() const;
    };

    Vec2f operator-(const Vec2f rhs);
    Vec2f operator*(const Vec2f& lhs, const float rhs);
    Vec2f operator*(const float lhs, const Vec2f& rhs);
}