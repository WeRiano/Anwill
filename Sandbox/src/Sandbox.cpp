#include <iostream>

#include "Tests/Renderer/RendererHelloWorld"
#include "Tests/Renderer/TextureHelloWorld.h"
#include "Tests/Renderer/FontHelloWorld.h"
#include "Tests/Renderer/BatchRendererHelloWorld.h"
#include "Tests/Renderer/SpriteAnimationTest.h"
#include "Tests/Gui/GuiTest.h"
#include "Tests/Physics/dynamics/DynamicsRender.h"
#include "Tests/Physics/dynamics/DynamicsTest.h"
#include "Tests/Physics/collision/CollisionRender.h"
#include "Tests/Physics/collision/CollisionTest.h"
#include "Tests/Ecs/EcsTestLayer.h"
#include "SmallerProjects/TopDownShadow/TPS.h"
#include "SmallerProjects/TopDownShadow/TPSCalcs.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox(const Anwill::WindowSettings& ws)
        : App(ws, true)
    {
        /// Renderer tests
        //AddLayer<RendererHelloWorld>(0);
        //AddLayer<TextureHelloWorld>(0, ws);
        //AddLayer<FontHelloWorld>(0, ws);
        //AddLayer<BatchRendererHelloWorld>(0, ws);
        AddLayer<SpriteAnimationTest>(0, ws);

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
};

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    WindowSettings ws{1200, 900, "Sandbox"};

    // Make this more apparent. What is required exactly?
    Anwill::Renderer::SetAPI(Anwill::GraphicsAPI::API::OpenGL);
    return new Sandbox(ws);
}

#include "EntryPoint.h" // Get the main function