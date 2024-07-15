#pragma once

#include "Anwill.h"
#include "SandboxEvents.h"

class SandboxLayer : public Anwill::Layer
{
public:
    SandboxLayer(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    /**
     * Spawn the window with sandbox test environments.
     */
    static void CreateSandboxWindow(const Anwill::Shared <Anwill::GuiContainer>& editorWindow);
    static void EnterTestEnvironment(SandboxEnvironmentEvent::Env environment,
                                     const Anwill::Shared <Anwill::GuiContainer>& window);

    static Anwill::Shared<Anwill::GuiText> s_GuiFpsText;
    static Anwill::Shared<Anwill::GuiText> s_AppFpsText;
};