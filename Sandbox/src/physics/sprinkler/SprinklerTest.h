#pragma once

#include "Anwill.h"

class SprinklerTest : public Anwill::Layer
{
public:
    SprinklerTest(unsigned int ups);

    void Update(const Anwill::Timestamp& timestamp) override;
    void ImguiUpdate() override;

private:
    Anwill::Timestamp m_SpawnTimeCount;
    float m_ObjSize;
    float m_TimeMultiplier;
    static constexpr double s_SpawnDeltaSeconds = 0.15;

    void SpawnObject();
    void TickObjects(Anwill::Timestamp delta);
    void KillObjects();
};
