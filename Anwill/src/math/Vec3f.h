#pragma once

namespace Anwill::Math {

    class Vec3f
    {
    public:
        Vec3f();
        Vec3f(float x, float y, float z);

        float GetX() const;
        float GetY() const;
        float GetZ() const;
    private:
        float m_X, m_Y, m_Z;
    };
}