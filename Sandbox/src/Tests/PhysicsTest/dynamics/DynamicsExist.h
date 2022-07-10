#pragma once

#include "Anwill.h"

class DynamicsExist : public Anwill::Layer
{
public:
    DynamicsExist(unsigned int ups);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Timestamp m_SpawnTimeCount;

    void SpawnObject();
    void KillObjects();
};
