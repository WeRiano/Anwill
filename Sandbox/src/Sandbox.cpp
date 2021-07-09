#include <iostream>

#include "Layer2D.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox()
    {
        AddLayer<Layer2D>(144);
    }
};

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    return new Sandbox();
}

#include "EntryPoint.h" // Get the main function
