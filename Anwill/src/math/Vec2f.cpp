#include <cmath>

#include "Vec2f.h"

namespace Anwill::Math {

    Vec2f::Vec2f()
        : m_X(0.0f), m_Y(0.0f)
    {}

    Vec2f::Vec2f(float x, float y)
        : m_X(x), m_Y(y)
    {}

    Vec2f Vec2f::operator*(const float s) const
    {
        Vec2f res(m_X, m_Y);
        res.m_X *= s;
        res.m_Y *= s;
        return res;
    }

    Vec2f Vec2f::operator+(const Vec2f& other) const
    {
        Vec2f res(m_X, m_Y);
        res.m_X += other.m_X;
        res.m_Y += other.m_Y;
        return res;
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
}