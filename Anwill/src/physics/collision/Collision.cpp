#include "Collision.h"

namespace Anwill {

    bool CollisionTest::CheckCollision(const RBody& body1, const Math::Mat4f& transform1, const RBody& body2,
                                       const Math::Mat4f& transform2, CollisionData& colData)
    {
        if( !body1.HasCollider() or !body2.HasCollider() ) {
            return false;
        }
        return body1.GetCollider()->CollisionCheck(transform1, body2.GetCollider().get(),
                                                   transform2, colData);
    }

    void CollisionTest::ResolveCollision(RBody& body1, Math::Mat4f& transform1, RBody& body2, Math::Mat4f& transform2,
                                         const CollisionData& collisionData)
    {
        Math::Vec3f normal = collisionData.normal;
        Math::Vec3f relVel = body2.GetVelocity() - body1.GetVelocity(); // Might have to swap

        if (normal.DotProduct(body1.GetPosition() - body2.GetPosition()) < 0.0f) {
            normal = -normal;
        }

        float e = 0.8f; // Idk
        float j = -(1.0f + e) * relVel.DotProduct(normal);
        j /= (1.0f / body1.GetMass()) + (1.0f / body2.GetMass());

        if(body1.IsStatic() and !body2.IsStatic()) {
            body2.ApplyImpulse(j * 2.0f, normal, false);
            body2.Move(-normal * (collisionData.depth));
        } else if(body2.IsStatic() and !body1.IsStatic())
        {
            body1.ApplyImpulse(j * 2.0f, normal, true);
            body1.Move(normal * (collisionData.depth));
        } else if(!body1.IsStatic() and !body2.IsStatic()) {
            body1.ApplyImpulse(j, normal, true);
            body2.ApplyImpulse(j, normal, false);
            body1.Move(normal * (collisionData.depth / 2.0f));
            body2.Move(-normal * (collisionData.depth / 2.0f));
        }
        // Two static bodies should not be able to collide, but we account for it anyway just in case :)
    }

    Math::Vec3f CollisionTest::GetArithmeticMean(const std::vector<Math::Vec3f>& vertices, Math::Mat4f transform)
    {
        Math::Vec3f sum;
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            sum += vertices[i];
        }
        sum = sum / (float) vertices.size();
        return sum;
    }
}