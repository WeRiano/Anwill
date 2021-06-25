#pragma once

#include "core/PlatDef.h"
#include "core/App.h"

//extern Anwill::App* Anwill::CreateApp();

int main()
{
    Anwill::Log::Init();

    Anwill::CreateApp();
}