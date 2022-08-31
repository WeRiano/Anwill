#pragma once

#include <vector>

#include "physics/collision/Collider.h"

namespace Anwill {

    /// **Convex** Polygon Collider
    class PolygonCollider : public Collider
    {
    public:
        /*
        // These two could use polymorphism instead if we need to
        PolygonCollider(float triLeftLength, float triRightLength, float triBaseLength);
        PolygonCollider(float quadWidth, float quadHeight);
         */

        PolygonCollider(std::vector<Math::Vec2f> vertices);
        PolygonCollider(const std::vector<Math::Vec3f>& vertices);

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

        /**
        * Projects the polygon onto an axis.
        * @param axis for the vertices to be projected onto
        * @param transform which the vertices are multiplied by
        * @param min value as a result of the projection
        * @param max value as a result of the projection
        */
        void Project(const Math::Vec2f& axis, const Math::Mat4f& transform,
                     float& min, float& max) const override;

        /**
         * @param otherCollider
         * @param thisTransform
         * @param otherTransform
         * @param colData
         * @return false if the two objects are not colliding according to the
         *         SAT algorithm projected onto the normals of the polygons edges
         *         (defined by m_Vertices). true otherwise.
         */
        bool SATCollision(const Collider* otherCollider,
                          const Math::Mat4f& thisTransform,
                          const Math::Mat4f& otherTransform,
                          CollisionData& colData) const;

        // Returns the vertex closest to the given reference point
        Math::Vec2f GetClosestVertex(const Math::Vec2f& point,
                                     const Math::Mat4f& transform) const;
    protected:
        std::vector<Math::Vec2f> m_Vertices;

        PolygonCollider() = default;
    };

    class QuadCollider : public PolygonCollider
    {
    public:
        QuadCollider();
        QuadCollider(float width, float height);
    };

    /*
    class TriCollider : public PolygonCollider
    {
    public:
        TriCollider();
    };
     */
}
