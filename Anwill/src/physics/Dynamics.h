#pragma once

#include "math/Vec3f.h"
#include "physics/collision/Collider.h"

namespace Anwill {

    /**
     * Law I: Every object in motion will remain in motion unless an external force acts on it
     * Law II: Force equals mass times acceleration: F = ma
     * Law III: For every action there is an equal and opposite reaction
     */
    class RBody {
    public:
        RBody();
        RBody(float mass, bool isStatic,
              Math::Vec3f position = {},
              Math::Vec3f velocity = {},
              Math::Vec3f force = {},
              Math::Vec3f gravity = {0.0f, -9.81f, 0.0f}
              );

        void ApplyForce(Math::Vec3f force);
        // We are applying a change in momentum == impulse,
        // but its instantaneous so I'm not sure if its accurate to
        // call it an 'impulse'.
        void ApplyImpulse(Math::Vec3f impulse, bool subtract = false);
        void Tick(float deltaSeconds);
        void Move(Math::Vec3f deltaPos);

        bool IsStatic() const;
        void SetStatic(bool IsStatic);
        float GetMass() const;
        Math::Vec3f GetPosition() const;
        Math::Vec3f GetVelocity() const;
        void SetVelocity(Math::Vec3f velocity);
        void SetGravAcc(Math::Vec3f gravAcc);
        std::shared_ptr<Collider> GetCollider() const;
        template <class C, typename... Args>
        void EmplaceCollider(Args... args)
        {
            m_Collider = std::make_shared<C>(std::forward<Args>(args)...);
        }
        bool HasCollider() const;

    private:
        bool m_Static;
        float m_M;
        // position, velocity, (temporary) net force
        // and gravitational acceleration
        Math::Vec3f m_P, m_V, m_F, m_G;
        std::shared_ptr<Collider> m_Collider;
    };
}
