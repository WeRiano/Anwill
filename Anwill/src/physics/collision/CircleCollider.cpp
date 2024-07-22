#include "CircleCollider.h"
#include "PolygonCollider.h"

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

    bool CircleCollider::CollisionCheck(const Math::Mat4f& thisTransform, const PolygonCollider* polyCollider,
                                        const Math::Mat4f& otherTransform, CollisionData& colData) const
    {
        return (SATCollision(polyCollider, thisTransform, otherTransform, colData) and
                polyCollider->SATCollision(this, otherTransform, thisTransform, colData));
    }

    bool CircleCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                        const CircleCollider* circleCollider,
                                        const Math::Mat4f& otherTransform,
                                        CollisionData& colData) const
    {
        // Simple circle collision check, nothing complicated.

        Math::Vec2f thisCentre = thisTransform * m_Centre;
        Math::Vec2f otherCentre = otherTransform * m_Centre;

        float bodyDistance = (thisCentre - otherCentre).GetLength();
        float scale = thisTransform.GetScale().X; // TODO: Ellipses
        float collisionDistance = (m_Radius * scale + circleCollider->m_Radius * scale);
        if( bodyDistance <= collisionDistance) {
            Math::Vec2f temp = thisCentre - otherCentre;
            colData.normal = {temp.X, temp.Y, 0.0f};
            colData.normal.Normalize();
            colData.depth = collisionDistance - bodyDistance;
            return true;
        } else {
            return false;
        }
    }

    void CircleCollider::Project(const Math::Vec2f& axis,
                                       const Math::Mat4f& transform,
                                       float& min, float& max) const
    {
        Math::Vec2f axisCopy = axis;
        axisCopy.Normalize();
        float scale = transform.GetScale().X; // TODO: Ellipses
        min = ((transform * m_Centre) - (axisCopy * m_Radius * scale)).ScalarProjection(axis);
        max = ((transform * m_Centre) + (axisCopy * m_Radius * scale)).ScalarProjection(axis);
    }

    bool CircleCollider::SATCollision(const PolygonCollider* otherCollider, const Math::Mat4f& thisTransform,
                                      const Math::Mat4f& otherTransform, CollisionData& colData) const
    {
        Math::Vec2f closestPolyVertex = otherCollider->GetClosestVertex(thisTransform * m_Centre,
                                                                        otherTransform);
        Math::Vec2f axis = closestPolyVertex - (thisTransform * m_Centre);
        axis.Normalize();

        float thisMin, thisMax, otherMin, otherMax;
        Project(axis, thisTransform, thisMin, thisMax);
        otherCollider->Project(axis, otherTransform, otherMin, otherMax);
        if (!OverlapCheck(thisMin, thisMax, otherMin, otherMax))
        {
            // If there is not an overlap <=> there is a gap, we know there is not a collision
            return false;
        }

        float axisDepth = std::min<float>(otherMax - thisMin, thisMax - otherMin);
        if (axisDepth < colData.depth)
        {
            colData.depth = axisDepth;
            colData.normal = Math::Vec3f(axis.X, axis.Y, 0.0f);
            colData.normal.Normalize();
        }
        // There is a collision according to the SAT algorithm projected onto the axes 'created' by this polygon.
        return true;
    }
}