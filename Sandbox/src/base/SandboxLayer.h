#pragma once

#include "Anwill.h"
#include "SandboxEvents.h"

class SandboxLayer : public Anwill::Layer
{
public:
    SandboxLayer(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;
    void ImguiUpdate() override;

private:
    void StartTestEnv(SandboxEnvironmentEvent::Env env);

    bool m_IsRunningTestEnv;
};