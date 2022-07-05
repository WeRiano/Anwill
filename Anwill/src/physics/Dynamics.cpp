#include "Dynamics.h"

namespace Anwill {

    RBody::RBody()
        : m_M(1.0f)
    {}

    RBody::RBody(float mass, Math::Vec3f position, Math::Vec3f velocity, Math::Vec3f force, Math::Vec3f gravity)
        : m_M(mass), m_P(position), m_V(velocity), m_F(force), m_G(gravity)
    {}

    void RBody::ApplyForce(Math::Vec3f force)
    {
        // F = ma
        m_F += force;
    }

    void RBody::ApplyImpulse(Math::Vec3f impulse)
    {
        // p = mv
        m_V += impulse / m_M;
    }

    void RBody::Tick(float deltaSeconds)
    {
        m_F += (m_G * m_M);
        m_V += ((m_F / m_M) * deltaSeconds);
        m_P += (m_V * deltaSeconds);

        m_F = Math::Vec3f();
    }

    Math::Vec3f RBody::GetPosition() const
    {
        return m_P;
    }

    void RBody::SetGravAcc(Math::Vec3f gravAcc)
    {
        m_G = gravAcc;
    }
}