#include "Anwill.h"
#include "EditorLayer.h"
#include "EditorEventHandler.h"

#include "ecs/EcsTestLayer.h"
#include "physics/collision/CollisionRender.h"
#include "physics/collision/CollisionTest.h"
#include "physics/dynamics/DynamicsRender.h"
#include "physics/dynamics/DynamicsTest.h"
#include "renderer/RendererHelloWorld.h"
#include "renderer/BatchRendererHelloWorld.h"
#include "renderer/FontHelloWorld.h"
#include "renderer/SpriteAnimationTest.h"
#include "renderer/TextureHelloWorld.h"
#include "tps/TPS.h"
#include "tps/TPSCalcs.h"

namespace Anwill {

    class AnwillEditor : public App
    {
    public:
        AnwillEditor(const Anwill::AppSettings& settings)
            : App(settings)
        {
            EditorEventHandler::Init();

            EditorEventHandler::Subscribe<StartTestEnvironmentEvent>(
                AW_BIND_THIS_MEMBER_FUNC(OnStartTestEnvironmentEvent));

            // Editor layer
            AddLayer<EditorLayer>(0, settings.GetWindowSettings());

            windowSettings = settings.GetWindowSettings();

            /// Renderer tests
            //AddLayer<RendererHelloWorld>(0, ws);
            //AddLayer<TextureHelloWorld>(0, ws);
            //AddLayer<FontHelloWorld>(0, ws);
            //AddLayer<BatchRendererHelloWorld>(0, ws);
            //AddLayer<SpriteAnimationTest>(0, ws);

            /// Physics tests
            //AddLayer<DynamicsTest>(10);
            //AddLayer<DynamicsRender>(0, ws);
            //AddLayer<CollisionRender>(0, ws);
            //AddLayer<CollisionTest>(30, ws);

            /// GUI tests
            //AddLayer<GuiTest>(0);

            /// ECS tests
            //AddLayer<EcsTestLayer>(144);

            /// Smaller projects
            //AddLayer<TPSCalcs>(30);
            //AddLayer<TPS>(0, 80.0f, 80.0f, 12, 10, ws);
        }

        inline void OnStartTestEnvironmentEvent(std::unique_ptr<Event>& event)
        {
            auto e = static_cast<StartTestEnvironmentEvent&>(*event);
            switch (e.GetEnv())
            {
                case StartTestEnvironmentEvent::Env::Ecs:
                    AddLayer<EcsTestLayer>(0);
                    break;
                case StartTestEnvironmentEvent::Env::PhysicsCollision:
                    AddLayer<DynamicsTest>(10);
                    AddLayer<DynamicsRender>(0, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::PhysicsDynamics:
                    AddLayer<CollisionRender>(0, windowSettings);
                    AddLayer<CollisionTest>(30, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::RendererHelloWorld:
                    AddLayer<RendererHelloWorld>(0, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::Font:
                    AddLayer<FontHelloWorld>(0, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::BatchRenderer:
                    AddLayer<BatchRendererHelloWorld>(0, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::SpriteAnimation:
                    AddLayer<SpriteAnimationTest>(0, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::Texture:
                    AddLayer<TextureHelloWorld>(0, windowSettings);
                    break;
                case StartTestEnvironmentEvent::Env::TopDownShooter:
                    AddLayer<TPSCalcs>(30);
                    AddLayer<TPS>(0, 80.0f, 80.0f, 12, 10,
                                  windowSettings);
                    break;
            }
        }

    private:
        WindowSettings windowSettings;
    };
}

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    Anwill::AppSettings appSettings = AppSettings()
        .SetTitle("Anwill Editor")
        .SetWindowResolution(1920, 1080)
        .SetGraphicsAPI(GraphicsAPI::API::OpenGL)
        .SetNumSamples(8);

    return new AnwillEditor(appSettings);
}

#include "EntryPoint.h" // Get the main function