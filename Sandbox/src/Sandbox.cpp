#include <iostream>

#include "Anwill.h"

#include "ecs/EcsTestLayer.h"
#include "gui/GuiTest.h"
#include "physics/dynamics/DynamicsRender.h"
#include "physics/dynamics/DynamicsTest.h"
#include "physics/collision/CollisionRender.h"
#include "physics/collision/CollisionTest.h"
#include "renderer/RendererHelloWorld"
#include "renderer/TextureHelloWorld.h"
#include "renderer/FontHelloWorld.h"
#include "renderer/BatchRendererHelloWorld.h"
#include "renderer/SpriteAnimationTest.h"
#include "tps/TPS.h"
#include "tps/TPSCalcs.h"

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
        AddLayer<BatchRendererHelloWorld>(0, ws);
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