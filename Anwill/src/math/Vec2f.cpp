#include <cmath>

#include "Vec2f.h"
#include "Vec3f.h"

namespace Anwill::Math {

    Vec2f::Vec2f()
        : m_X(0.0f), m_Y(0.0f)
    {}

    Vec2f::Vec2f(float x, float y)
        : m_X(x), m_Y(y)
    {}

    Vec2f& Vec2f::operator=(const Vec3f& other)
    {
        m_X = other.GetX();
        m_Y = other.GetY();
        return *this;
    }

    Vec2f Vec2f::operator+(const Vec2f& other) const
    {
        return {m_X + other.m_X, m_Y + other.m_Y};
    }

    Vec2f Vec2f::operator-(const Vec2f& other) const
    {
        return {m_X - other.m_X, m_Y - other.m_Y};
    }

    void Vec2f::operator+=(const Vec2f& other)
    {
        m_X += other.m_X;
        m_Y += other.m_Y;
    }

    void Vec2f::operator*=(const float other)
    {
        m_X *= other;
        m_Y *= other;
    }

    Vec2f operator-(const Vec2f rhs)
    {
        return {rhs.GetX(), rhs.GetY()};
    }

    Vec2f operator*(const Vec2f& lhs, const float rhs)
    {
        return {lhs.GetX() * rhs, lhs.GetY() * rhs};
    }

    Vec2f Math::operator*(const float lhs, const Vec2f& rhs)
    {
        return {lhs * rhs.GetX(),lhs * rhs.GetY()};
    }

    Vec2f::operator Vec3f() const
    {
        return {m_X, m_Y, 0.0f};
    }

    float Vec2f::ScalarProjection(Vec2f vec2f) const
    {
        vec2f.Normalize();
        return DotProduct(vec2f);
    }

    float Vec2f::DotProduct(const Vec2f& other) const
    {
        return m_X * other.m_X + m_Y * other.m_Y;
    }

    void Vec2f::Move(float deltaX, float deltaY)
    {
        m_X += deltaX;
        m_Y += deltaY;
    }

    void Vec2f::Normalize()
    {
        float length = GetLength();
        m_X = m_X / length;
        m_Y = m_Y / length;
    }

    void Vec2f::Negate()
    {
        m_X = -m_X;
        m_Y = -m_Y;
    }

    void Vec2f::Clamp(const Vec2f& min, const Vec2f& max)
    {
        if(m_X < min.m_X) { m_X = min.m_X; }
        if(m_X > max.m_X) { m_X = max.m_X; }
        if(m_Y < min.m_Y) { m_Y = min.m_Y; }
        if(m_Y > max.m_Y) { m_Y = max.m_Y; }
    }

    float Vec2f::GetLength() const
    {
        return sqrtf(m_X*m_X + m_Y*m_Y);
    }

    float Vec2f::GetX() const
    {
        return m_X;
    }

    float Vec2f::GetY() const
    {
        return m_Y;
    }

    std::string Vec2f::ToString() const
    {
        return std::string("[" + std::to_string(GetX()) + ", " + std::to_string(GetY()) + "]");
    }
}