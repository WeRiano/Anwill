#include <limits>

#include "PolygonCollider.h"
#include "CircleCollider.h"

namespace Anwill {

    PolygonCollider::PolygonCollider(std::vector<Math::Vec2f> vertices)
        : m_Vertices(std::move(vertices))
    {}

    PolygonCollider::PolygonCollider(const std::vector<Math::Vec3f>& vertices)
    {
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            m_Vertices.emplace_back(vertices[i].X, vertices[i].Y);
        }
    }

    bool PolygonCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                         const Collider* collider,
                                         const Math::Mat4f& otherTransform,
                                         CollisionData& colData) const
    {
        return collider->CollisionCheck(otherTransform, this, thisTransform, colData);
    }

    bool PolygonCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                         const PolygonCollider* polyCollider,
                                         const Math::Mat4f& otherTransform,
                                         CollisionData& colData) const
    {
        return (SATCollision(polyCollider, thisTransform, otherTransform, colData) and
            polyCollider->SATCollision(this, otherTransform, thisTransform, colData));
    }

    bool PolygonCollider::CollisionCheck(const Math::Mat4f& thisTransform,
                                         const CircleCollider* circleCollider,
                                         const Math::Mat4f& otherTransform,
                                         CollisionData& colData) const
    {
        return (SATCollision(circleCollider, thisTransform, otherTransform, colData) and
                circleCollider->SATCollision(this, otherTransform, thisTransform, colData));
    }

    Math::Vec2f PolygonCollider::GetClosestVertex(const Math::Vec2f& point, const Math::Mat4f& transform) const
    {
        Math::Vec2f closest = transform * m_Vertices[0];
        float closestLength = (point - (transform * m_Vertices[0])).GetLength();
        for(unsigned int i = 1; i < m_Vertices.size(); i++)
        {
            Math::Vec2f temp = transform * m_Vertices[i];
            float len = (point - temp).GetLength();
            if (len < closestLength)
            {
                closest = temp;
                closestLength = len;
            }
        }
        return closest;
    }

    void PolygonCollider::Project(const Math::Vec2f& axis,
                                         const Math::Mat4f& transform,
                                         float& min, float& max) const
    {
        min = std::numeric_limits<float>::max();
        max = std::numeric_limits<float>::min();

        for (unsigned int i = 0; i < m_Vertices.size(); i++)
        {
            float projValue = (transform * m_Vertices[i]).ScalarProjection(axis);
            if (projValue < min)
            {
                min = projValue;
            }
            if (projValue > max)
            {
                max = projValue;
            }
        }
    }

    bool PolygonCollider::SATCollision(const Collider* otherCollider,
                                       const Math::Mat4f& thisTransform,
                                       const Math::Mat4f& otherTransform,
                                       CollisionData& colData) const
    {
        for(unsigned int i = 0; i < m_Vertices.size(); i++)
        {
            Math::Vec2f edge = (thisTransform * m_Vertices[i]) - (thisTransform * m_Vertices[(i + 1) % m_Vertices.size()]);
            Math::Vec2f axis = {-edge.Y, edge.X}; // Vector perpendicular to the edge
            axis.Normalize();

            float thisMin, thisMax, otherMin, otherMax;
            Project(axis,thisTransform, thisMin, thisMax);
            otherCollider->Project(axis, otherTransform, otherMin, otherMax);
            if (!OverlapCheck(thisMin, thisMax, otherMin, otherMax))
            {
                // If there is not an overlap <=> there is a gap, we know there is not a collision
                return false;
            }

            float axisDepth = std::min<float>(otherMax - thisMin, thisMax - otherMin);
            if(axisDepth < colData.depth)
            {
                colData.depth = axisDepth;
                colData.normal = Math::Vec3f(axis.X, axis.Y, 0.0f);
                colData.normal.Normalize();
            }
        }
        // There is a collision according to the SAT algorithm projected onto the axes 'created' by this polygon.
        return true;
    }

    QuadCollider::QuadCollider()
    {
        m_Vertices = {
                Math::Vec2f(-0.5f, -0.5f),
                Math::Vec2f(-0.5f,  0.5f),
                Math::Vec2f( 0.5f,  0.5f),
                Math::Vec2f( 0.5f, -0.5f)
        };
    }

    QuadCollider::QuadCollider(float width, float height)
    {
        m_Vertices = {
            Math::Vec2f(-0.5f * width, -0.5f * height),
            Math::Vec2f(-0.5f * width,  0.5f * height),
            Math::Vec2f( 0.5f * width,  0.5f * height),
            Math::Vec2f( 0.5f * width, -0.5f * height)
        };
    }
}