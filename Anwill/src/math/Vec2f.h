#pragma once

namespace Anwill::Math {

    class Vec2f
    {
    public:
        Vec2f();
        Vec2f(float x, float y);

        void Move(float deltaX, float deltaY);
        void Normalize();
        const float  GetLength();
        const float GetX() const;
        const float GetY() const;
    private:
        float m_X, m_Y;
    };

}