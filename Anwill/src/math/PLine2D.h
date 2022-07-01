#pragma once

#include "Vec2f.h"

namespace Anwill::Math {

    class PLine2D
    {
    public:
        PLine2D(Vec2f p, Vec2f d);

        bool Intersects(const PLine2D& other, float& t1, float& t2);
    private:
        Vec2f m_P, m_D;
    };

}