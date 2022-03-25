#pragma once

#include <array>

#include "Anwill.h"

class EcsTestLayer : public Anwill::Layer
{
public:
    EcsTestLayer(const unsigned int ups);

    virtual void Update(const Anwill::Timestamp &timestamp) override;

private:
    std::array<Anwill::EntityID, 10> m_Entities;
};