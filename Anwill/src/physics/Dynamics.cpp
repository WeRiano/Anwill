#include "Dynamics.h"

namespace Anwill {

    RBody::RBody()
        : m_Static(false), m_M(1.0f)
    {}

    RBody::RBody(float mass, bool isStatic, Math::Vec3f position, Math::Vec3f velocity, Math::Vec3f force, Math::Vec3f gravity)
        : m_Static(isStatic), m_M(mass), m_P(position), m_V(velocity), m_F(force), m_G(gravity)
    {}

    void RBody::ApplyForce(Math::Vec3f force)
    {
        // F = ma
        m_F += force;
    }

    void RBody::ApplyImpulse(Math::Vec3f impulse, bool subtract)
    {
        // p = mv
        subtract ? m_V -= impulse / m_M : m_V += impulse / m_M;
    }

    void RBody::Tick(float deltaSeconds)
    {
        m_F += (m_G * m_M);
        m_V += ((m_F / m_M) * deltaSeconds);
        m_P += (m_V * deltaSeconds);

        m_F = Math::Vec3f();
    }

    void RBody::Move(Math::Vec3f deltaPos)
    {
        m_P += deltaPos;
    }

    bool RBody::IsStatic() const
    {
        return m_Static;
    }

    void RBody::SetStatic(bool IsStatic)
    {
        m_Static = IsStatic;
    }

    float RBody::GetMass() const
    {
        return m_M;
    }

    Math::Vec3f RBody::GetPosition() const
    {
        return m_P;
    }

    Math::Vec3f RBody::GetVelocity() const
    {
        return m_V;
    }

    void RBody::SetVelocity(Math::Vec3f velocity)
    {
        m_V = velocity;
    }

    std::shared_ptr<Collider> RBody::GetCollider() const
    {
        return m_Collider;
    }

    void RBody::SetGravAcc(Math::Vec3f gravAcc)
    {
        m_G = gravAcc;
    }

    bool RBody::HasCollider() const
    {
        return !(m_Collider == nullptr);
    }
}