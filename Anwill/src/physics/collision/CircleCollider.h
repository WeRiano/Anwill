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
                            const PolygonCollider* polyCollider,
                            const Math::Mat4f& otherTransform,
                            CollisionData& colData) const override;
        bool CollisionCheck(const Math::Mat4f& thisTransform,
                            const CircleCollider* circleCollider,
                            const Math::Mat4f& otherTransform,
                            CollisionData& colData) const override;

        void ProjectCircle(const Math::Vec2f& axis, const Math::Mat4f& transform, float& min, float& max) const;
        bool SATCollision(const PolygonCollider* otherCollider,
                          const Math::Mat4f& thisTransform,
                          const Math::Mat4f& otherTransform,
                          CollisionData& colData) const;
    private:
        float m_Radius;
        Math::Vec2f m_Centre;
    };
}
