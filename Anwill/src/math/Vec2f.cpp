#include <cmath>

#include "Vec2f.h"

namespace Anwill::Math {

    Vec2f::Vec2f()
        : m_X(0.0f), m_Y(0.0f)
    {}

    Vec2f::Vec2f(float x, float y)
        : m_X(x), m_Y(y)
    {}

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

    const float Vec2f::GetLength()
    {
        return sqrtf(m_X*m_X + m_Y*m_Y);
    }

    const float Vec2f::GetX() const
    {
        return m_X;
    }

    const float Vec2f::GetY() const
    {
        return m_Y;
    }
}