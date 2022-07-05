#include "PhyExist.h"

PhyExist::PhyExist(unsigned int ups)
    : Anwill::Layer(ups), m_SpawnTimeCount(1000000 * 0.1)
{}

void PhyExist::Update(const Anwill::Timestamp& timestamp)
{
    m_SpawnTimeCount += timestamp.DeltaAbs(m_LastUpdate);
    if (m_SpawnTimeCount.GetSeconds() >= 0.1) {
        m_SpawnTimeCount = Anwill::Timestamp(0);
        SpawnObject();
    }
    KillObjects();

    Layer::Update(timestamp);
}

void PhyExist::SpawnObject()
{
    Anwill::EntityID newID = Anwill::Ecs::CreateEntity();
    Anwill::Ecs::AddComponent<Anwill::RBody>(newID, 1.5f);
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(newID);
    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::MeshComponent>([newID, this](Anwill::EntityID id, Anwill::RBody& rBody,
            Anwill::MeshComponent& meshComponent){
        if(id == newID)
        {
            //auto push = Anwill::Math::Vec3f(4. * 80000.0f, 4. * 100000.0f, 0.0f);
            auto push = Anwill::Math::Vec3f(150.0f, 200.0f, 0.0f);

            // TODO: Create Random module for engine
            float randAngle = Anwill::Random::GetUniformFloat(-20.0f, 20.0f);

            push = Anwill::Math::Mat4f::Identity().RotateZ(randAngle) * push;

            rBody.ApplyImpulse(push);
            //rBody.ApplyForce(push);
            meshComponent.mesh = Anwill::Mesh::CreateRectMesh(20.0f, 20.0f);
        }
    });
}

void PhyExist::KillObjects()
{
    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::MeshComponent>([](Anwill::EntityID id, Anwill::RBody& rBody, Anwill::MeshComponent& meshComp){
        if ((rBody.GetPosition().GetY() <= -10.0f) or (rBody.GetPosition().GetX() >= 1500.0f))
        {
            Anwill::Ecs::RemoveEntity(id);
        }
    });
}
