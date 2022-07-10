#pragma once

#include <memory>
#include <limits>

#include "math/Mat4f.h"

#undef max

namespace Anwill {

    class RectangleCollider;
    class CircleCollider;
    class TriangleCollider;

    struct CollisionData {
        CollisionData()
            : depth(std::numeric_limits<float>::max())
        {}
        Math::Vec3f normal;
        float depth;
    };

    class Collider {
    public:
        // We use raw pointers in these functions for the double dispatch pattern
        // (so we can supply 'this' as a parameter).
        virtual bool CollisionCheck(const Math::Mat4f& thisTransform,
                                    const Collider* collider,
                                    const Math::Mat4f& otherTransform,
                                    CollisionData& colData) const = 0;

        virtual bool CollisionCheck(const Math::Mat4f& thisTransform,
                                    const RectangleCollider* rectCollider,
                                    const Math::Mat4f& otherTransform,
                                    CollisionData& colData) const = 0;
        virtual bool CollisionCheck(const Math::Mat4f& thisTransform,
                                    const CircleCollider* circleCollider,
                                    const Math::Mat4f& otherTransform,
                                    CollisionData& colData) const = 0;
    };
}