#include <iostream>

#include "Tests/Renderer/RendererHelloWorld"
#include "Tests/PhysicsTest/PhyDelta.h"
#include "Tests/PhysicsTest/PhyExist.h"
#include "Tests/EcsTest/EcsTestLayer.h"
#include "SmallerProjects/TopDownShadow/TPS.h"
#include "SmallerProjects/TopDownShadow/TPSCalcs.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox(const Anwill::WindowSettings& ws)
        : App(ws)
    {
        //AddLayer<RenderHelloWorld>(0);

        //AddLayer<EcsTestLayer>(144);

        AddLayer<TPSCalcs>(30);
        AddLayer<TPS>(0, 80.0f, 80.0f, 12, 10, ws);

        //AddLayer<PhyDelta>(0, ws);
        //AddLayer<PhyExist>(10);
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