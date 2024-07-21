#include "Sandbox.h"
#include "base/SandboxLayer.h"
#include "base/SandboxEventHandler.h"

#include "ecs/EcsStressTest.h"
#include "physics/arena/ArenaRender.h"
#include "physics/arena/ArenaTest.h"
#include "physics/sprinkler/SprinklerRender.h"
#include "physics/sprinkler/SprinklerTest.h"
#include "renderer/PrimitivesTest.h"
#include "renderer/BatchRendererTest.h"
#include "renderer/FontTest.h"
#include "renderer/SpriteAnimationTest.h"
#include "renderer/TextureTest.h"
#include "tps/TPS.h"
#include "tps/TPSCalcs.h"

Sandbox::Sandbox(const Anwill::AppSettings& settings)
    : App(settings), m_ActiveEnv(SandboxEnvironmentEvent::Env::None)
{
    SandboxEventHandler::Init();

    SandboxEventHandler::Subscribe<SandboxEnvironmentEvent>(
            AW_BIND_THIS_MEMBER_FUNC(OnStartTestEnvironmentEvent)
    );

    // Editor layer
    AddLayer<SandboxLayer>(0, settings.GetWindowSettings());

    m_WindowSettings = settings.GetWindowSettings();
}

void Sandbox::OnStartTestEnvironmentEvent(std::unique_ptr<Anwill::Event>& event)
{
    auto e = static_cast<SandboxEnvironmentEvent&>(*event);
    if(e.GetEnv() == SandboxEnvironmentEvent::Env::None)
    {
        EndTestEnvironment();
    } else
    {
        StartTestEnvironment(e);
    }
}

void Sandbox::StartTestEnvironment(SandboxEnvironmentEvent event)
{
    m_ActiveEnv = event.GetEnv();
    switch (m_ActiveEnv)
    {
        case SandboxEnvironmentEvent::Env::EcsStressTest:
            AddLayer<EcsStressTest>(0);
            break;
        case SandboxEnvironmentEvent::Env::PhysicsCollision:
            AddLayer<SprinklerTest>(10);
            AddLayer<SprinklerRender>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::PhysicsDynamics:
            AddLayer<ArenaRender>(0, m_WindowSettings);
            AddLayer<ArenaTest>(30, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::Primitives:
            AddLayer<PrimitivesTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::Font:
            AddLayer<FontTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::BatchRenderer:
            AddLayer<BatchRendererTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::SpriteAnimation:
            AddLayer<SpriteAnimationTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::Texture:
            AddLayer<TextureTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::TopDownShadow:
            AddLayer<TPSCalcs>(30);
            AddLayer<TPS>(0, 80.0f, 80.0f, m_WindowSettings.width / 80.0f, m_WindowSettings.height / 80.0f,
                          m_WindowSettings);
            break;
    }
}

void Sandbox::EndTestEnvironment()
{
    switch (m_ActiveEnv)
    {
        case SandboxEnvironmentEvent::Env::EcsStressTest:
            RemoveLayer<EcsStressTest>();
            break;
        case SandboxEnvironmentEvent::Env::PhysicsCollision:
            RemoveLayer<SprinklerTest>();
            RemoveLayer<SprinklerRender>();
            break;
        case SandboxEnvironmentEvent::Env::PhysicsDynamics:
            RemoveLayer<ArenaRender>();
            RemoveLayer<ArenaTest>();
            break;
        case SandboxEnvironmentEvent::Env::Primitives:
            RemoveLayer<PrimitivesTest>();
            break;
        case SandboxEnvironmentEvent::Env::Font:
            RemoveLayer<FontTest>();
            break;
        case SandboxEnvironmentEvent::Env::BatchRenderer:
            RemoveLayer<BatchRendererTest>();
            break;
        case SandboxEnvironmentEvent::Env::SpriteAnimation:
            RemoveLayer<SpriteAnimationTest>();
            break;
        case SandboxEnvironmentEvent::Env::Texture:
            RemoveLayer<TextureTest>();
            break;
        case SandboxEnvironmentEvent::Env::TopDownShadow:
            RemoveLayer<TPSCalcs>();
            RemoveLayer<TPS>();
            break;
    }
    m_ActiveEnv = SandboxEnvironmentEvent::Env::None;
}
