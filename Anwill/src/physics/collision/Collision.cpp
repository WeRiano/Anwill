#include "Collision.h"

namespace Anwill {

    bool Collision::CheckCollision(const RBody& body1, const Math::Mat4f& transform1, const RBody& body2,
                                   const Math::Mat4f& transform2, CollisionData& colData)
    {
        if( !body1.HasCollider() or !body2.HasCollider() ) {
            return false;
        }
        return body1.GetCollider()->CollisionCheck(transform1, body2.GetCollider().get(),
                                                   transform2, colData);
    }

    void Collision::ResolveCollision(RBody& body1, Math::Mat4f& transform1, RBody& body2, Math::Mat4f& transform2,
                                     const CollisionData& collisionData)
    {
        /*
        Math::Vec3f vel1 = body1.GetVelocity();
        vel1.Negate();
        Math::Vec3f vel2 = body2.GetVelocity();
        vel2.Negate();
        float netXMomentum = body1.GetVelocity().GetX() * body1.GetMass() + body2.GetVelocity().GetX() * body2.GetMass();
        float netYMomentum = body1.GetVelocity().GetY() * body1.GetMass() + body2.GetVelocity().GetY() * body2.GetMass();
         */

        Math::Vec3f normal = collisionData.normal;
        if (normal.DotProduct(body1.GetPosition() - body2.GetPosition()) < 0.0f) {
            normal = -normal;
        }

        body1.Move(normal * (collisionData.depth / 2.0f));
        body2.Move(-normal * (collisionData.depth / 2.0f));

        //body1.SetVelocity(-body1.GetVelocity());
        //body2.SetVelocity(-body2.GetVelocity());

        // Since we swap body1 and body2 due to double dispatch the normal is

        // A really shitty but easy "solution" perhaps
        //body1.SetVelocity(vel1);
        //body2.SetVelocity(vel2);

    }

    Math::Vec3f Collision::GetArithmeticMean(const std::vector<Math::Vec3f>& vertices, Math::Mat4f transform)
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