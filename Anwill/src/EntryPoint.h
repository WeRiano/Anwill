#pragma once

#include <memory>

#include "core/PlatDef.h"
#include "core/App.h"

int main()
{
    Anwill::Log::Init();

    Anwill::App* app = Anwill::CreateApp();
    app->Run();
    // TODO: app->Terminate(); ?
    delete app;
}