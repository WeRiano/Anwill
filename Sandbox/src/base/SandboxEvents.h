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
        TopDownShooter
    };

    explicit SandboxEnvironmentEvent(Env env, bool isStarting)
        : m_Env(env), m_IsStarting(isStarting)
    {}
    inline Env GetEnv() const { return m_Env; }
    inline bool IsStarting() const { return m_IsStarting; }

private:
    Env m_Env;
    // False = ending
    bool m_IsStarting;
};