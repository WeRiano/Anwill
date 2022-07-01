#include "Vec3f.h"

namespace Anwill::Math {

    Vec3f::Vec3f()
        : m_X(0.0f), m_Y(0.0f), m_Z(0.0f)
    {}

    Vec3f::Vec3f(float x, float y, float z)
        : m_X(x), m_Y(y), m_Z(z)
    {}

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