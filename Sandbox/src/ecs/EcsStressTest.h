#pragma once

#include <array>

#include "Anwill.h"

struct Comp1 {};
struct Comp2 {};
struct Comp3 {};
struct Comp4 {};
struct Comp5 {};

class EcsStressTest : public Anwill::Layer
{
public:
    EcsStressTest(const unsigned int ups);

    virtual void Update(const Anwill::Timestamp &timestamp) override;
    void ImguiUpdate() override;

private:
    std::vector<Anwill::EntityID> m_Entities;
    bool m_Started;
    int m_NumEntities, m_NumComponents;
    Anwill::Timestamp m_Duration;

    void RegisterComponents() const;
    void AddComponentsToEntity(Anwill::EntityID entityID) const;
    void IterateThroughEntities();
};