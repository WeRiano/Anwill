#include "CircleCollider.h"

namespace Anwill {

    CircleCollider::CircleCollider(float radius, Math::Vec2f centre)
        : m_Radius(radius), m_Centre(centre)
    {}

    bool CircleCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                        const Collider* collider,
                                        const Math::Mat4f& otherTransform,
                                        CollisionData& colData) const
    {
        return collider->CollisionCheck(otherTransform, this, thisTransform, colData);
    }

    bool CircleCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                        const RectangleCollider* rectCollider,
                                        const Math::Mat4f& otherTransform,
                                        CollisionData& colData) const
    {
        return true;
        return false;
    }

    bool CircleCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                        const CircleCollider* circleCollider,
                                        const Math::Mat4f& otherTransform,
                                        CollisionData& colData) const
    {
        Math::Vec2f thisCentre = thisTransform * m_Centre;
        Math::Vec2f otherCentre = otherTransform * m_Centre;

        float bodyDistance = (thisCentre - otherCentre).GetLength();
        float collisionDistance = (m_Radius + circleCollider->m_Radius);
        if( bodyDistance <= collisionDistance) {
            Math::Vec2f temp = thisCentre - otherCentre;
            colData.normal = {temp.GetX(), temp.GetY(), 0.0f};
            colData.normal.Normalize();
            colData.depth = collisionDistance - bodyDistance;
            return true;
        } else {
            return false;
        }
    }
}