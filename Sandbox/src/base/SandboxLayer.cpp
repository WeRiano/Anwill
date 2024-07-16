#include "SandboxLayer.h"
#include "SandboxEventHandler.h"
#include "core/AppStats.h"
#include "ecs/Ecs.h"

Anwill::Shared<Anwill::GuiText> SandboxLayer::s_GuiFpsText, SandboxLayer::s_AppFpsText;

SandboxLayer::SandboxLayer(unsigned int ups, const Anwill::WindowSettings& ws)
        : Layer(ups)
{
    auto editorWindow = Anwill::Gui::CreateWindow("Editor");
    CreateSandboxWindow(editorWindow);
}

void SandboxLayer::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);

    if(s_GuiFpsText != nullptr)
    {
        s_GuiFpsText->SetText("GUI renders at " + Anwill::Math::RoundToString(Anwill::Gui::GetRenderFramesPerSecond(), 1) +
                              " fps (" + Anwill::Math::RoundToString(Anwill::Gui::GetRenderFrameTimeMS(), 1) +
                              " ms per frame)");
    }
    if(s_AppFpsText != nullptr)
    {
        s_AppFpsText->SetText("App updates " + Anwill::Math::RoundToString(Anwill::AppStats::GetAppUpdatesPerSecond(), 1) +
                              " times per second (" + Anwill::Math::RoundToString(Anwill::AppStats::GetAppUpdateTimeMS(), 1) +
                              " ms per cycle)");
    }

    SandboxEventHandler::Pop();
}

void SandboxLayer::CreateSandboxWindow(const Anwill::Shared<Anwill::GuiContainer>& editorWindow)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.3f, 0.3f});

    editorWindow->Clear();
    Anwill::Gui::Button("Ecs test", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::Ecs, editorWindow);
    }, true, editorWindow);

    Anwill::Gui::Dropdown("Physics");
    Anwill::Gui::Button("Collisions", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::PhysicsCollision, editorWindow);
    });
    Anwill::Gui::Button("Dynamics", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::PhysicsDynamics, editorWindow);
    });

    auto renderingDropdown = Anwill::Gui::Dropdown("Rendering", editorWindow);
    Anwill::Gui::Button("Primitives", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::Primitives, editorWindow);
    });

    Anwill::Gui::Button("Font", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::Font, editorWindow);
    }, true, renderingDropdown);

    Anwill::Gui::Button("Batch renderer", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::BatchRenderer, editorWindow);
    });

    Anwill::Gui::Button("Sprite animation", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::SpriteAnimation, editorWindow);
    });

    Anwill::Gui::Button("Texture", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::Texture, editorWindow);
    });

    Anwill::Gui::Button("Top Down Shooter", [editorWindow]() {
        EnterTestEnvironment(SandboxEnvironmentEvent::Env::TopDownShooter, editorWindow);
    }, true, editorWindow);
}

void SandboxLayer::EnterTestEnvironment(SandboxEnvironmentEvent::Env environment,
                                        const Anwill::Shared<Anwill::GuiContainer>& window)
{
    // Push layer(s) to app
    SandboxEnvironmentEvent event(environment, true);
    SandboxEventHandler::Add(event);

    // Set window state
    window->Clear();
    s_GuiFpsText = Anwill::Gui::Text("", true, window);
    s_AppFpsText = Anwill::Gui::Text("");
    Anwill::Gui::Button("Return", [window, environment](){
        SandboxEnvironmentEvent endEvent(environment, false);
        SandboxEventHandler::Add(endEvent);
        CreateSandboxWindow(window);
    });
    // Gui::Hide // <-- Prevent other layers from using this window
}