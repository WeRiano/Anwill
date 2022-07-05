#pragma once

namespace Anwill::Math {

    class Vec2f
    {
    public:
        Vec2f();
        Vec2f(float x, float y);

        Vec2f operator*(const float s) const;
        Vec2f operator+(const Vec2f& other) const;

        void Move(float deltaX, float deltaY);
        void Normalize();
        float GetLength() const;
        float GetX() const;
        float GetY() const;
    private:
        float m_X, m_Y;
    };

}