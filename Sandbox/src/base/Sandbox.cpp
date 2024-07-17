#include "Sandbox.h"
#include "base/SandboxLayer.h"
#include "base/SandboxEventHandler.h"

#include "ecs/EcsTestLayer.h"
#include "physics/collision/CollisionRender.h"
#include "physics/collision/CollisionTest.h"
#include "physics/dynamics/DynamicsRender.h"
#include "physics/dynamics/DynamicsTest.h"
#include "renderer/PrimitivesTest.h"
#include "renderer/BatchRendererHelloWorld.h"
#include "renderer/FontHelloWorld.h"
#include "renderer/SpriteAnimationTest.h"
#include "renderer/TextureHelloWorld.h"
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
        case SandboxEnvironmentEvent::Env::Ecs:
            AddLayer<EcsTestLayer>(0);
            break;
        case SandboxEnvironmentEvent::Env::PhysicsCollision:
            AddLayer<DynamicsTest>(10);
            AddLayer<DynamicsRender>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::PhysicsDynamics:
            AddLayer<CollisionRender>(0, m_WindowSettings);
            AddLayer<CollisionTest>(30, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::Primitives:
            AddLayer<PrimitivesTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::Font:
            AddLayer<FontHelloWorld>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::BatchRenderer:
            AddLayer<BatchRendererHelloWorld>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::SpriteAnimation:
            AddLayer<SpriteAnimationTest>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::Texture:
            AddLayer<TextureHelloWorld>(0, m_WindowSettings);
            break;
        case SandboxEnvironmentEvent::Env::TopDownShooter:
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
        case SandboxEnvironmentEvent::Env::Ecs:
            RemoveLayer<EcsTestLayer>();
            break;
        case SandboxEnvironmentEvent::Env::PhysicsCollision:
            RemoveLayer<DynamicsTest>();
            RemoveLayer<DynamicsRender>();
            break;
        case SandboxEnvironmentEvent::Env::PhysicsDynamics:
            RemoveLayer<CollisionRender>();
            RemoveLayer<CollisionTest>();
            break;
        case SandboxEnvironmentEvent::Env::Primitives:
            RemoveLayer<PrimitivesTest>();
            break;
        case SandboxEnvironmentEvent::Env::Font:
            RemoveLayer<FontHelloWorld>();
            break;
        case SandboxEnvironmentEvent::Env::BatchRenderer:
            RemoveLayer<BatchRendererHelloWorld>();
            break;
        case SandboxEnvironmentEvent::Env::SpriteAnimation:
            RemoveLayer<SpriteAnimationTest>();
            break;
        case SandboxEnvironmentEvent::Env::Texture:
            RemoveLayer<TextureHelloWorld>();
            break;
        case SandboxEnvironmentEvent::Env::TopDownShooter:
            RemoveLayer<TPSCalcs>();
            RemoveLayer<TPS>();
            break;
    }
    m_ActiveEnv = SandboxEnvironmentEvent::Env::None;
}
