#include "Anwill.h"
#include "EditorLayer.h"

class AnwillEditor : public Anwill::App
{
public:
    AnwillEditor(const Anwill::WindowSettings& ws)
            : App(ws)
    {

        // Editor layer
        AddLayer<EditorLayer>(0, ws);

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

    static inline void CenterWindow();
    static inline Anwill::Math::Vec2f
};

Anwill::App* Anwill::CreateApp()
{
    AnwillEditor::

    // Define graphics API, Window Settings and more
    WindowSettings ws{1200, 900, 16, "Editor"};

    // Make this more apparent. What is required exactly?
    Anwill::Renderer::SetAPI(Anwill::GraphicsAPI::API::OpenGL);
    return new AnwillEditor(ws);
}

#include "EntryPoint.h" // Get the main function