#include "events/Event.h"

namespace Anwill {

    class StartTestEnvironmentEvent : public Event
    {
    public:
        enum class Env
        {
            Ecs,
            PhysicsCollision,
            PhysicsDynamics,
            RendererHelloWorld,
            BatchRenderer,
            Font,
            SpriteAnimation,
            Texture,
            TopDownShooter
        };

        explicit StartTestEnvironmentEvent(Env env)
            : m_Env(env)
        {}
        inline Env GetEnv() const { return m_Env; }

    private:
        Env m_Env;
    };
}