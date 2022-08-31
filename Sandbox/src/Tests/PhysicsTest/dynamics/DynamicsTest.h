#pragma once

#include "Anwill.h"

class DynamicsTest : public Anwill::Layer
{
public:
    DynamicsTest(unsigned int ups);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Timestamp m_SpawnTimeCount;
    static constexpr double s_SpawnDeltaSeconds = 0.5;

    void SpawnObject();
    void KillObjects();
};
