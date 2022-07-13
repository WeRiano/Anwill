#pragma once

#include <vector>

#include "physics/collision/PolygonCollider.h"
#include "physics/collision/CircleCollider.h"
#include "physics/Dynamics.h"

namespace Anwill {

    class CollisionTest
    {
    public:
        static bool CheckCollision(const RBody& body1, const Math::Mat4f& transform1,
                                   const RBody& body2, const Math::Mat4f& transform2,
                                   CollisionData& colData);

        // Resolving a collision assumes that body1 and body2 have NOT swapped positions,
        // i.e body1 from CheckCollision is body2 in ResolveCollision and vise-versa.
        static void ResolveCollision(RBody& body1, Math::Mat4f& transform1, RBody& body2, Math::Mat4f& transform2,
                                     const CollisionData& collisionData);
    private:
        static Math::Vec3f GetArithmeticMean(const std::vector<Math::Vec3f>& vertices, Math::Mat4f transform);
    };
}
