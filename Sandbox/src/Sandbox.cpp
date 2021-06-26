#include <iostream>

#include "Anwill.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox()
    {
        std::cout << "Hello Anwill!" << std::endl;
    }
};

Anwill::App* Anwill::CreateApp()
{
    return new Sandbox();
}

#include "EntryPoint.h" // Get the main function
