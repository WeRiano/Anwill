#pragma once

#include "events/Event.h"

/**
 * Marks that a test environment has begun or is terminated
 */
class SandboxEnvironmentEvent : public Anwill::Event
{
public:
    enum class Env
    {
        Ecs,
        PhysicsCollision,
        PhysicsDynamics,
        Primitives,
        BatchRenderer,
        Font,
        SpriteAnimation,
        Texture,
        TopDownShooter,

        None
    };

    explicit SandboxEnvironmentEvent(Env env)
        : m_Env(env)
    {}
    inline Env GetEnv() const { return m_Env; }

private:
    Env m_Env;
};