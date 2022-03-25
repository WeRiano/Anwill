#include "EcsTestLayer.h"

EcsTestLayer::EcsTestLayer(const unsigned int ups)
        : Layer(ups)
{
    Anwill::Ecs::Initialize();

    for(unsigned int i = 0; i < m_Entities.size(); i++)
    {
        m_Entities[i] = Anwill::Ecs::CreateEntity();
        AW_INFO("Entity ID: {}", m_Entities[i]);
    }

    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(m_Entities[0],
                                                     0, 13.0f, Anwill::TestObject(-1));
    Anwill::Ecs::AddComponent<Anwill::CameraComponent>(m_Entities[0], 4);
}

void EcsTestLayer::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);
    AW_INFO("{0}", timestamp.GetSeconds());
    Anwill::Ecs::ForEach<Anwill::MeshComponent, Anwill::CameraComponent>([](Anwill::EntityID id,
                                                                            auto& mesh,
                                                                            auto& camera){
    });
}
