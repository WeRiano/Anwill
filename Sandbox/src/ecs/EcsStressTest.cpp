#include "EcsStressTest.h"

EcsStressTest::EcsStressTest(const unsigned int ups)
        : Layer(ups), m_Started(false), m_NumEntities(10000), m_NumComponents(3),
          m_Duration(0)
{}

void EcsStressTest::Update(const Anwill::Timestamp& timestamp)
{
    if(m_Started) {
        IterateThroughEntities();
    }

    Layer::Update(timestamp);
}

void EcsStressTest::ImguiUpdate()
{
    if(!m_Started) {
        ImGui::SliderInt("Entities", &m_NumEntities, 1, 50000);
        ImGui::SliderInt("Components", &m_NumComponents, 1, 5);
        if (ImGui::Button("Start stress test")) {
            RegisterComponents();
            m_Started = true;
            AW_DEBUG("Starting ECS stress test.");
            for(int i = 0; i < m_NumEntities; i++) {
                //m_Entities.push_back(Anwill::Ecs::CreateEntity());
                auto id = Anwill::Ecs::CreateEntity();
                AddComponentsToEntity(id);
            }
        }
    } else {
        Anwill::EcsStats ecsStats = Anwill::Ecs::GetStats();
        ImGui::Text("Stress testing ecs...");
        ImGui::Text("Registered component types: %d", ecsStats.numRegisteredComponents);
        ImGui::Text("Alive entities: %d", ecsStats.numAliveEntities);
        ImGui::Text("Alive components: %d", ecsStats.numComponents);
        ImGui::Text("Each iteration takes %.2Lf MS", m_Duration.GetMilliseconds());
        if (ImGui::Button("Reset")) {
            m_Started = false;
            Anwill::Ecs::Reset();
        }
    }


}

void EcsStressTest::RegisterComponents() const
{
    int k = 0;
    switch(m_NumComponents)
    {
        case 5:
            Anwill::Ecs::RegisterComponent<Comp5>();
        case 4:
            Anwill::Ecs::RegisterComponent<Comp4>();
        case 3:
            Anwill::Ecs::RegisterComponent<Comp3>();
        case 2:
            Anwill::Ecs::RegisterComponent<Comp2>();
        case 1:
            Anwill::Ecs::RegisterComponent<Comp1>();
            break;
    }
}

void EcsStressTest::AddComponentsToEntity(Anwill::EntityID entityID) const
{
    switch(m_NumComponents)
    {
        case 5:
            Anwill::Ecs::AddComponent<Comp5>(entityID);
        case 4:
            Anwill::Ecs::AddComponent<Comp4>(entityID);
        case 3:
            Anwill::Ecs::AddComponent<Comp3>(entityID);
        case 2:
            Anwill::Ecs::AddComponent<Comp2>(entityID);
        case 1:
            Anwill::Ecs::AddComponent<Comp1>(entityID);
            break;
    }
}

void EcsStressTest::IterateThroughEntities()
{
    int k = 0;
    Anwill::Timestamp start;
    switch(m_NumComponents)
    {
        case 5:
            Anwill::Ecs::ForEach<Comp1, Comp2, Comp3, Comp4, Comp5>(
                    [&k](Anwill::EntityID id, Comp1 c1, Comp2 c2, Comp3 c3, Comp4 c4, Comp5 c5) {
                        k++;
                    });
            break;
        case 4:
            Anwill::Ecs::ForEach<Comp1, Comp2, Comp3, Comp4>(
                    [&k](Anwill::EntityID id, Comp1 c1, Comp2 c2, Comp3 c3, Comp4 c4) {
                        k++;
                    });
            break;
        case 3:
            Anwill::Ecs::ForEach<Comp1, Comp2, Comp3>(
                    [&k](Anwill::EntityID id, Comp1 c1, Comp2 c2, Comp3 c3) {
                        k++;
                    });
            break;
        case 2:
            Anwill::Ecs::ForEach<Comp1, Comp2>(
                    [&k](Anwill::EntityID id, Comp1 c1, Comp2 c2) {
                        k++;
                    });
            break;
        case 1:
            Anwill::Ecs::ForEach<Comp1>(
                    [&k](Anwill::EntityID id, Comp1 c1) {
                        k++;
                    });
            break;
    }
    Anwill::Timestamp end;
    m_Duration = end - start;
}
