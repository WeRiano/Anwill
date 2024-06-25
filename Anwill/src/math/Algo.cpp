#include "Algo.h"

namespace Anwill::Math {

    bool Algo::IsPointInsideRectangle(const Math::Vec2f& a, const Math::Vec2f& b,
                                      const Math::Vec2f& c, const Math::Vec2f& d,
                                      const Math::Vec2f& point)
    {
        float first = (b-a).DotProduct(point-a);
        float second = (c-b).DotProduct(point-b);

        return ( (0.0f <= first) && (first <= (b-a).DotProduct(b-a)) ) &&
               ( (0.0f <= second) && (second <= (c-b).DotProduct(c-b)) );
    }

    bool Algo::IsPointInsideCircle(const Vec2f& centre, float radius, const Vec2f& point)
    {
        return (point.X < centre.X + radius && point.X > centre.X - radius) &&
               (point.Y < centre.Y + radius && point.Y > centre.Y - radius);
    }
}