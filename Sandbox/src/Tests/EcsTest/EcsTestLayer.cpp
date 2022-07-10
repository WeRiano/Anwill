#include "EcsTestLayer.h"

EcsTestLayer::EcsTestLayer(const unsigned int ups)
        : Layer(ups)
{

    for(unsigned int i = 0; i < m_Entities.size(); i++)
    {
        m_Entities[i] = Anwill::Ecs::CreateEntity();
        AW_INFO("Entity ID: {}", m_Entities[i]);
    }

    /*
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(m_Entities[0],0, 13.0f);
    Anwill::Ecs::AddComponent<Anwill::CameraComponent>(m_Entities[0], 4);
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(m_Entities[1],0, 42.0f);
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(m_Entities[2],0, 42.0f);
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(m_Entities[3],0, 42.0f);
    Anwill::Ecs::DeleteComponent<Anwill::MeshComponent>(m_Entities[2]);
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(m_Entities[4],0, 42.0f);
     */
}

void EcsTestLayer::Update(const Anwill::Timestamp& timestamp)
{
    /*
    Layer::Update(timestamp);
    Anwill::Ecs::ForEach<Anwill::MeshComponent, Anwill::CameraComponent>([](Anwill::EntityID id,
                                                                            Anwill::MeshComponent& mesh,
                                                                            Anwill::CameraComponent& camera) {
        std::cout << id << std::endl;
    });
    Anwill::Ecs::ForEach<Anwill::MeshComponent>([](Anwill::EntityID id,
                                                                            Anwill::MeshComponent& mesh) {
        std::cout << id << std::endl;
    });
    */
}
