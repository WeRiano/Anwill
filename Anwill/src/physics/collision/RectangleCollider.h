#pragma once

#include "math/Vec2f.h"
#include "physics/collision/Collider.h"

namespace Anwill {

    // DEPRECATED: Use polygon collider
    class RectangleCollider : public Collider
    {
    public:
        RectangleCollider(Math::Vec2f bl, Math::Vec2f tl, Math::Vec2f tr, Math::Vec2f br);

        bool CollisionCheck(const Math::Mat4f& thisTransform,
                            const Collider* collider,
                            const Math::Mat4f& otherTransform,
                            CollisionData& colData) const override;

        bool CollisionCheck(const Math::Mat4f& thisTransform,
                            const PolygonCollider* rectCollider,
                            const Math::Mat4f& otherTransform,
                            CollisionData& colData) const override;
        bool CollisionCheck(const Math::Mat4f& thisTransform,
                            const RectangleCollider* rectCollider,
                            const Math::Mat4f& otherTransform,
                            CollisionData& colData) const override;
        bool CollisionCheck(const Math::Mat4f& thisTransform,
                            const CircleCollider* circleCollider,
                            const Math::Mat4f& otherTransform,
                            CollisionData& colData) const override;

    private:
        Math::Vec2f m_BL, m_TL, m_TR, m_BR;

        bool CheckOverlap(float min1, float max1, float min2, float max2) const;
        bool GapCheck(const Math::Vec2f& axis, const RectangleCollider& other,
                      const Math::Mat4f& thisTransform, const Math::Mat4f& otherTransform, CollisionData& colData) const;
    };
}
