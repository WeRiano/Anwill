#pragma once

#include "math/Vec3f.h"

namespace Anwill {

    /**
     * Law I: Every object in motion will remain in motion unless an external force acts on it
     * Law II: Force equals mass times acceleration: F = ma
     * Law III: For every action there is an equal and opposite reaction
     */
    class RBody {
    public:
        RBody();
        RBody(float mass,
              Math::Vec3f position = {},
              Math::Vec3f velocity = {},
              Math::Vec3f force = {},
              Math::Vec3f gravity = {0.0f, -9.81f, 0.0f}
              );

        void ApplyForce(Math::Vec3f force);
        // We are applying a change in momentum == impulse, but its instantaneous so I'm not sure if its accurate to
        // call it an 'impulse'.
        void ApplyImpulse(Math::Vec3f impulse);
        void Tick(float deltaSeconds);
        Math::Vec3f GetPosition() const;
        void SetGravAcc(Math::Vec3f gravAcc);

    private:
        float m_M;
        Math::Vec3f m_P, m_V, m_F, m_G; // position, velocity, (temporary) net force and gravitational acceleration
    };
}
