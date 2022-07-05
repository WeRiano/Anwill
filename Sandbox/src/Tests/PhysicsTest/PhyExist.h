#pragma once

#include "Anwill.h"

class PhyExist : public Anwill::Layer
{
public:
    PhyExist(unsigned int ups);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Timestamp m_SpawnTimeCount;

    void SpawnObject();
    void KillObjects();
};
