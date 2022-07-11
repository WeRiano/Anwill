#include "RectangleCollider.h"

namespace Anwill {

    RectangleCollider::RectangleCollider(Math::Vec2f bl, Math::Vec2f tl, Math::Vec2f tr, Math::Vec2f br)
        : m_BL(bl), m_TL(tl), m_TR(tr), m_BR(br)
    {}

    bool RectangleCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                           const Collider* collider,
                                           const Math::Mat4f& otherTransform,
                                           CollisionData& colData) const
    {
        return collider->CollisionCheck(otherTransform, this, thisTransform, colData);
    }

    bool RectangleCollider::CollisionCheck(const Math::Mat4f& thisTransform, const PolygonCollider* polyCollider,
                                           const Math::Mat4f& otherTransform, CollisionData& colData) const
    {
        return false;
    }

    bool RectangleCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                           const RectangleCollider* rectCollider,
                                           const Math::Mat4f& otherTransform,
                                           CollisionData& colData) const
    {
        Math::Vec2f x1 = thisTransform.Get2DBasisVectors()[0];
        Math::Vec2f y1 = thisTransform.Get2DBasisVectors()[1];
        Math::Vec2f x2 = otherTransform.Get2DBasisVectors()[0];
        Math::Vec2f y2 = otherTransform.Get2DBasisVectors()[1];

        bool gapA = GapCheck(x1, *rectCollider, thisTransform, otherTransform, colData);
        bool gapB = GapCheck(y1, *rectCollider, thisTransform, otherTransform, colData);
        bool gapC = GapCheck(x2, *rectCollider, thisTransform, otherTransform, colData);
        bool gapD = GapCheck(y2, *rectCollider, thisTransform, otherTransform, colData);

        return !(gapA or gapB or gapC or gapD);
    }

    bool RectangleCollider::CheckOverlap(float min1, float max1, float min2, float max2) const
    {
        return !(max2 < min1 or max1 < min2);
    }

    bool RectangleCollider::GapCheck(const Math::Vec2f& axis, const RectangleCollider& other,
                                     const Math::Mat4f& thisTransform, const Math::Mat4f& otherTransform,
                                     CollisionData& colData) const
    {
        float bl = (thisTransform * m_BL).ScalarProjection(axis);
        float tl = (thisTransform * m_TL).ScalarProjection(axis);
        float tr = (thisTransform * m_TR).ScalarProjection(axis);
        float br = (thisTransform * m_BR).ScalarProjection(axis);

        float obl = (otherTransform * other.m_BL).ScalarProjection(axis);
        float otl = (otherTransform * other.m_TL).ScalarProjection(axis);
        float otr = (otherTransform * other.m_TR).ScalarProjection(axis);
        float obr = (otherTransform * other.m_BR).ScalarProjection(axis);

        // For some reason just supplying n values is so much more tedious ...
        float min1 = std::min<float>(bl, std::min<float>(tl, std::min<float>(tr, br)));
        float max1 = std::max<float>(bl, std::max<float>(tl, std::max<float>(tr, br)));
        float min2 = std::min<float>(obl, std::min<float>(otl, std::min<float>(otr, obr)));
        float max2 = std::max<float>(obl, std::max<float>(otl, std::max<float>(otr, obr)));

        bool thereIsAGap = !CheckOverlap(min1, max1, min2, max2);
        if(thereIsAGap) // Completely unnecessary to gather collision data if there is a gap <=> no collision
        {
            return true;
        }

        float axisDepth = std::min<float>(max2 - min1, max1 - min2);
        if(axisDepth < colData.depth)
        {
            colData.depth = axisDepth;
            colData.normal = Math::Vec3f(axis.GetX(), axis.GetY(), 0.0f);
            colData.normal.Normalize();
        }
        return false;
    }

    bool RectangleCollider::CollisionCheck(const Math::Mat4f& thisTransform, const CircleCollider* circleCollider,
                                           const Math::Mat4f& otherTransform, CollisionData& colData) const
    {
        return false;
    }
}