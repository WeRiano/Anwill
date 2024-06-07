#pragma once

#include "Vec2f.h"

namespace Anwill::Math {

    class Algo
    {
    public:
        /**
         * @brief Points are clockwise.
         * @param a
         * @param b
         * @param c
         * @param d
         * @param point
         * @return
         */
        static bool IsPointInsideRectangle(const Math::Vec2f& a, const Math::Vec2f& b,
                                           const Math::Vec2f& c, const Math::Vec2f& d,
                                           const Math::Vec2f& point);
        static bool IsPointInsideCircle(const Math::Vec2f& centre,
                                        float radius,
                                        const Math::Vec2f& point);
    };

    void f();

    bool t();
}
