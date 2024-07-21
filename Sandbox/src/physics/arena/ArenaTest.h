#pragma once

#include "Anwill.h"

#include "ArenaRender.h"

class ArenaTest : public Anwill::Layer
{
public:
    ArenaTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::WindowSettings m_WS;

    void MoveAndTiltPlayer();
    void HandleCollisions();
    void SwapShape();
    void WrapBodies();
};
