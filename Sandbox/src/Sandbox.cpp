#include <iostream>

#include "Anwill.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox()
    {

    }
};

Anwill::App* Anwill::CreateApp()
{
    // Define graphics API, Window Settings and more
    return new Sandbox();
}

#include "EntryPoint.h" // Get the main function
