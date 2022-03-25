#include <iostream>

#include "Layer2D.h"
#include "EcsTestLayer.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox(const Anwill::WindowSettings ws)
        : App(ws)
    {
        //AddLayer<Layer2D>(144);
        AddLayer<EcsTestLayer>(144);
    }
};

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    WindowSettings ws{800, 600, "Sandbox"};
    Anwill::Renderer::SetAPI(Anwill::GraphicsAPI::API::OpenGL);
    return new Sandbox(ws);
}

#include "EntryPoint.h" // Get the main function