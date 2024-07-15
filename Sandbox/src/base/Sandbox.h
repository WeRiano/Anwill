#pragma once

#include "Anwill.h"
#include "SandboxEvents.h"

class Sandbox : public Anwill::App
{
public:
    Sandbox(const Anwill::AppSettings& settings);

private:
    Anwill::WindowSettings m_WindowSettings;

    void OnStartTestEnvironmentEvent(std::unique_ptr<Anwill::Event>& event);
    void StartTestEnvironment(SandboxEnvironmentEvent event);
    void EndTestEnvironment(SandboxEnvironmentEvent event);
};
