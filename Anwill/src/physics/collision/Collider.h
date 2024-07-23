#pragma once

#include <memory>
#include <limits>

#include "math/Mat4f.h"

#undef max

namespace Anwill {

    class CircleCollider;
    class PolygonCollider;

    struct CollisionData {
        CollisionData()
            : depth(std::numeric_limits<float>::max())
        {}
        Math::Vec3f normal;
        float depth, e = 0.8;
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
                                    const PolygonCollider* polyCollider,
                                    const Math::Mat4f& otherTransform,
                                    CollisionData& colData) const = 0;
        virtual bool CollisionCheck(const Math::Mat4f& thisTransform,
                                    const CircleCollider* circleCollider,
                                    const Math::Mat4f& otherTransform,
                                    CollisionData& colData) const = 0;

        virtual void Project(const Math::Vec2f& axis, const Math::Mat4f& transform, float& min, float& max) const = 0;
    protected:
        inline static bool OverlapCheck(float min1, float max1, float min2, float max2) {
            return !(max2 < min1 or max1 < min2);
        }
    };
}