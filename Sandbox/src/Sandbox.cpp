#include <iostream>

#include "Tests/Renderer/RendererHelloWorld"
#include "Tests/Renderer/TextureHelloWorld.h"
#include "Tests/Renderer/FontHelloWorld.h"
#include "Tests/Renderer/BatchRendererHelloWorld.h"
#include "Tests/PhysicsTest/dynamics/DynamicsRender.h"
#include "Tests/PhysicsTest/dynamics/DynamicsTest.h"
#include "Tests/PhysicsTest/collision/CollisionRender.h"
#include "Tests/PhysicsTest/collision/CollisionTest.h"
#include "Tests/EcsTest/EcsTestLayer.h"
#include "SmallerProjects/TopDownShadow/TPS.h"
#include "SmallerProjects/TopDownShadow/TPSCalcs.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox(const Anwill::WindowSettings& ws)
        : App(ws, true)
    {
        AddLayer<RendererHelloWorld>(0);
        //AddLayer<TextureHelloWorld>(0, ws);
        //AddLayer<FontHelloWorld>(0, ws);
        //AddLayer<BatchRendererHelloWorld>(0, ws);

        //AddLayer<EcsTestLayer>(144);

        //AddLayer<TPSCalcs>(30);
        //AddLayer<TPS>(0, 80.0f, 80.0f, 12, 10, ws);

        //AddLayer<DynamicsTest>(10);
        //AddLayer<DynamicsRender>(0, ws);

        //AddLayer<CollisionRender>(0, ws);
        //AddLayer<CollisionTest>(30, ws);
    }
};

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    WindowSettings ws{1200, 900, "Sandbox"};

    // Change? Just feed 1 struct with information to App or something.
    Anwill::Renderer::SetAPI(Anwill::GraphicsAPI::API::OpenGL);
    return new Sandbox(ws);
}

#include "EntryPoint.h" // Get the main function