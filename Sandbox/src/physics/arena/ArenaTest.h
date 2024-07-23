#pragma once

#include "Anwill.h"

#include "ArenaRender.h"

class ArenaTest : public Anwill::Layer
{
public:
    ArenaTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;
    void ImguiUpdate() override;

private:
    Anwill::WindowSettings m_WS;
    float m_VelocityMagnitude;
    float m_FrictionMultiplier;
    // Coefficient of restitution
    float m_CoR;

    void MoveAndRotatePlayer();
    void HandleCollisions();
    void WrapAndSlowBodies(const Anwill::Timestamp delta);
};
