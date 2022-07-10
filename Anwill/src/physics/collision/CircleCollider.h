#pragma once

#include "math/Vec2f.h"
#include "physics/collision/Collider.h"

namespace Anwill {

    class CircleCollider : public Collider
    {
    public:
        CircleCollider(float radius, Math::Vec2f centre = {});

        bool CollisionCheck(const Math::Mat4f& thisTransform,
                            const Collider* collider,
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
        float m_Radius;
        Math::Vec2f m_Centre;
    };
}
