#pragma once

namespace Anwill::Math {

    class Vec3f;

    class Vec2f
    {
    public:
        Vec2f();
        Vec2f(float x, float y);

        Vec2f& operator=(const Vec3f& other);
        Vec2f operator+(const Vec2f& other) const;
        Vec2f operator-(const Vec2f& other) const;
        void operator+=(const Vec2f& other);

        float ScalarProjection(Vec2f vec2f) const;
        float DotProduct(const Vec2f& other) const;
        void Move(float deltaX, float deltaY);
        void Normalize();
        void Negate();
        float GetLength() const;
        float GetX() const;
        float GetY() const;
    private:
        float m_X, m_Y;
    };

    Vec2f operator-(const Vec2f rhs);
    Vec2f operator*(const Vec2f& lhs, const float rhs);
    Vec2f operator*(const float lhs, const Vec2f& rhs);
}