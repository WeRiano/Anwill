#include "DynamicsExist.h"

DynamicsExist::DynamicsExist(unsigned int ups)
    : Anwill::Layer(ups), m_SpawnTimeCount(1000000 * 0.15)
{}

void DynamicsExist::Update(const Anwill::Timestamp& timestamp)
{
    m_SpawnTimeCount += timestamp.DeltaAbs(m_LastUpdate);
    if (m_SpawnTimeCount.GetSeconds() >= 0.15) {
        m_SpawnTimeCount = Anwill::Timestamp(0);
        SpawnObject();
    }
    KillObjects();

    Layer::Update(timestamp);
}

void DynamicsExist::SpawnObject()
{
    Anwill::EntityID newID = Anwill::Ecs::CreateEntity();
    Anwill::Ecs::AddComponent<Anwill::TransformComponent>(newID);
    Anwill::Ecs::AddComponent<Anwill::RBody>(newID, 2.5f);
    Anwill::Ecs::AddComponent<Anwill::MeshComponent>(newID);
    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::MeshComponent>([newID](Anwill::EntityID id, Anwill::RBody& rBody,
            Anwill::MeshComponent& meshComponent){
        if(id == newID)
        {
            meshComponent.mesh = Anwill::Mesh::CreateRectMesh(20.0f, 20.0f);

            float randAngle = Anwill::Random::GetUniformFloat(-20.0f, 20.0f);
            auto push = Anwill::Math::Vec3f(150.0f, 200.0f, 0.0f);
            auto rotMat = Anwill::Math::Mat4f::RotateZ(Anwill::Math::Mat4f::Identity(), randAngle);
            push = rotMat * push;
            rBody.ApplyImpulse(push);

            // TODO: Less hardcoded for now. Could implement a VerticesToCollider method that belongs to
            //  either Collider or Mesh, or that is independent somehow.
            auto vs = meshComponent.mesh.GetVertices();
            rBody.SetCollider<Anwill::RectangleCollider>(Anwill::Math::Vec2f(vs[0].GetX(), vs[0].GetY()),
                                                         Anwill::Math::Vec2f(vs[1].GetX(), vs[1].GetY()),
                                                         Anwill::Math::Vec2f(vs[2].GetX(), vs[2].GetY()),
                                                         Anwill::Math::Vec2f(vs[3].GetX(), vs[3].GetY()));
        }
    });
}

void DynamicsExist::KillObjects()
{
    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::MeshComponent>([](Anwill::EntityID id, Anwill::RBody& rBody, Anwill::MeshComponent& meshComp){
        if ((rBody.GetPosition().GetY() <= -10.0f) or (rBody.GetPosition().GetX() >= 1500.0f))
        {
            Anwill::Ecs::RemoveEntity(id);
        }
    });


    Anwill::Ecs::ForEach<Anwill::TransformComponent, Anwill::RBody>([](Anwill::EntityID idA,
            Anwill::TransformComponent& transformA, Anwill::RBody& bodyA) {
        Anwill::Ecs::ForEach<Anwill::TransformComponent, Anwill::RBody>([&idA, &transformA, &bodyA](Anwill::EntityID idB,
                                                                           Anwill::TransformComponent& transformB,
                                                                           Anwill::RBody& bodyB) {
            if(idA != idB)
            {
                // TODO: Dont iterate twice (I think this is currently negating the col resolvement)
                Anwill::CollisionData data;
                if (Anwill::Collision::CheckCollision(bodyA, transformA.transform,
                                                      bodyB, transformB.transform, data))
                {
                    Anwill::Ecs::RemoveEntity(idA);
                    Anwill::Ecs::RemoveEntity(idB);
                }
            }
        });
    });
}
