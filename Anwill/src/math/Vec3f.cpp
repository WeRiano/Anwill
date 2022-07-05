#include <cmath>

#include "Vec3f.h"

namespace Anwill::Math {

    Vec3f::Vec3f()
        : m_X(0.0f), m_Y(0.0f), m_Z(0.0f)
    {}

    Vec3f::Vec3f(float x, float y, float z)
        : m_X(x), m_Y(y), m_Z(z)
    {}

    Vec3f Vec3f::operator+(const Vec3f& other) const
    {
        Vec3f res(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z);
        return res;
    }

    void Vec3f::operator+=(const Vec3f& other)
    {
        m_X += other.m_X;
        m_Y += other.m_Y;
        m_Z += other.m_Z;
    }

    void Vec3f::operator*=(const float s)
    {
        m_X *= s;
        m_Y *= s;
        m_Z *= s;
    }

    Vec3f Vec3f::operator/(const float d) const
    {
        return {m_X / d, m_Y / d, m_Z / d};
    }

    Vec3f operator*(const Vec3f& lhs, const float rhs)
    {
        return {lhs.GetX() * rhs, lhs.GetY() * rhs, lhs.GetZ() * rhs};
    }

    Vec3f operator*(const float lhs, const Vec3f& rhs)
    {
        return {rhs.GetX() * lhs, rhs.GetY() * lhs, rhs.GetZ() * lhs};
    }

    void Vec3f::Move(float deltaX, float deltaY, float deltaZ)
    {
        m_X += deltaX;
        m_Y += deltaY;
        m_Z += deltaZ;
    }

    void Vec3f::Normalize()
    {
        float length = GetLength();
        m_X = m_X / length;
        m_Y = m_Y / length;
        m_Z = m_Z / length;
    }

    float Vec3f::GetLength() const
    {
        return sqrtf(m_X*m_X + m_Y*m_Y + m_Z*m_Z);
    }

    float Vec3f::GetX() const
    {
        return m_X;
    }

    float Vec3f::GetY() const
    {
        return m_Y;
    }

    float Vec3f::GetZ() const
    {
        return m_Z;
    }
}