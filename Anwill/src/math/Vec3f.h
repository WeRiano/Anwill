#pragma once

namespace Anwill::Math {

    class Vec3f
    {
    public:
        Vec3f();
        Vec3f(float x, float y, float z);

        Vec3f operator+(const Vec3f& other) const;
        Vec3f operator-(const Vec3f& other) const;
        void operator+=(const Vec3f& other);
        void operator*=(const float s);
        Vec3f operator/(const float d) const;

        float DotProduct(const Vec3f& other) const;
        Vec3f CrossProduct(const Vec3f& other) const;
        void Move(float deltaX, float deltaY, float deltaZ);
        void Normalize();
        void Negate();
        float GetLength() const;
        float GetX() const;
        float GetY() const;
        float GetZ() const;
    private:
        float m_X, m_Y, m_Z;
    };

    Vec3f operator-(const Vec3f rhs);
    Vec3f operator*(const Vec3f& lhs, const float rhs);
    Vec3f operator*(const float lhs, const Vec3f& rhs);
}