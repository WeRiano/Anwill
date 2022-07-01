#include <iostream>

#include "Layer2D.h"
#include "EcsTestLayer.h"
#include "TPS.h"
#include "TPSCalcs.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox(const Anwill::WindowSettings& ws)
        : App(ws)
    {
        //AddLayer<Layer2D>(0);
        //AddLayer<EcsTestLayer>(144);
        AddLayer<TPSCalcs>(30);
        AddLayer<TPS>(0, 80.0f, 80.0f, 12, 10, ws);
    }
};

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    WindowSettings ws{1200, 900, "Sandbox"};
    Anwill::Renderer::SetAPI(Anwill::GraphicsAPI::API::OpenGL);
    return new Sandbox(ws);
}

#include "EntryPoint.h" // Get the main function