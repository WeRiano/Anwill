#include "EditorLayer.h"
#include "EditorEventHandler.h"
#include "ecs/Ecs.h"

namespace Anwill {

    EditorLayer::EditorLayer(unsigned int ups, const Anwill::WindowSettings& ws)
        : Layer(ups)
    {
        Renderer::SetClearColor({0.3f, 0.3f, 0.3f});

        CreateSandboxWindow();
    }

    void EditorLayer::Update(const Anwill::Timestamp& timestamp)
    {
        Layer::Update(timestamp);

        EditorEventHandler::Pop();
    }

    void EditorLayer::CreateSandboxWindow()
    {
        auto editorWindow = Gui::CreateWindow("Editor");

        Gui::Button("Ecs test", [editorWindow]() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::Ecs);
            EditorEventHandler::Add(event);
        });

        Gui::Dropdown("Physics");
        Gui::Button("Collisions", [editorWindow]() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::PhysicsCollision);
            EditorEventHandler::Add(event);
            Gui::EraseWindow(editorWindow);
        });
        Gui::Button("Dynamics", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::PhysicsDynamics);
            EditorEventHandler::Add(event);
        });

        Gui::Button("Renderer Hello World", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::RendererHelloWorld);
            EditorEventHandler::Add(event);
        }, true, editorWindow);

        Gui::Button("Font", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::Font);
            EditorEventHandler::Add(event);
        }, true, editorWindow);

        Gui::Button("Batch renderer", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::BatchRenderer);
            EditorEventHandler::Add(event);
        }, true, editorWindow);

        Gui::Button("Sprite animation", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::SpriteAnimation);
            EditorEventHandler::Add(event);
        }, true, editorWindow);

        Gui::Button("Texture", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::Texture);
            EditorEventHandler::Add(event);
        }, true, editorWindow);

        Gui::Button("Top Down Shooter", []() {
            StartTestEnvironmentEvent event(StartTestEnvironmentEvent::Env::TopDownShooter);
            EditorEventHandler::Add(event);
        }, true, editorWindow);
    }
}