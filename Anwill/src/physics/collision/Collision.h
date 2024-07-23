#pragma once

#include <vector>

#include "physics/collision/PolygonCollider.h"
#include "physics/collision/CircleCollider.h"
#include "physics/Dynamics.h"

namespace Anwill {

    class Collision
    {
    public:
        static bool Check(const RBody& body1, const Math::Mat4f& transform1,
                          const RBody& body2, const Math::Mat4f& transform2,
                          CollisionData& colData);

        // Resolving a collision assumes that body1 and body2 have NOT swapped positions,
        // i.e body1 from Check is body2 in Resolve and vise-versa.
        static void Resolve(RBody& body1, Math::Mat4f& transform1, RBody& body2,
                            Math::Mat4f& transform2,
                            const CollisionData& collisionData);
    };
}
