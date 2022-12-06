#pragma once

#include "Anwill.h"

#include "CollisionRender.h"

class CollisionTest : public Anwill::Layer
{
public:
    CollisionTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::WindowSettings m_WS;

    void MoveAndTiltPlayer();
    void HandleCollisions();
    void SwapShape();
    void WrapBodies();
};
