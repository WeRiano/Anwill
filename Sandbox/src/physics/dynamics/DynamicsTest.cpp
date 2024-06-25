#include "DynamicsTest.h"

DynamicsTest::DynamicsTest(unsigned int ups)
    : Anwill::Layer(ups), m_SpawnTimeCount(1000000 * s_SpawnDeltaSeconds)
{
    Anwill::Ecs::RegisterComponent<Anwill::RBody>();
    Anwill::Ecs::RegisterComponent<Anwill::Math::Mat4f>();
    Anwill::Ecs::RegisterComponent<Anwill::Mesh>();
}

void DynamicsTest::Update(const Anwill::Timestamp& timestamp)
{
    m_SpawnTimeCount += timestamp.DeltaAbs(m_LastUpdate);
    if (m_SpawnTimeCount.GetSeconds() >= s_SpawnDeltaSeconds) {
        m_SpawnTimeCount = Anwill::Timestamp(0);
        SpawnObject();
    }
    KillObjects();

    Layer::Update(timestamp);
}

void DynamicsTest::SpawnObject()
{
    float objWidth = 20.0f;
    float objHeight = 20.0f;
    Anwill::EntityID newID = Anwill::Ecs::CreateEntity();
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(newID,
                                       Anwill::Math::Mat4f::Scale(
                                               Anwill::Math::Mat4f::Identity(),
                                               {objWidth, objHeight, 0.0f}));
    Anwill::Ecs::AddComponent<Anwill::RBody>(newID, 2.5f, false);

    Anwill::Ecs::ForEntity<Anwill::RBody>(newID, []
    (Anwill::RBody& rBody){
        float randAngle = Anwill::Random::GetUniformFloat(-20.0f, 20.0f);
        auto pushVec = Anwill::Math::Vec3f(150.0f, 250.0f, 0.0f);
        auto rotMat = Anwill::Math::Mat4f::RotateZ(Anwill::Math::Mat4f::Identity(),
                                                   randAngle);
        pushVec = rotMat * pushVec;
        rBody.ApplyImpulse(pushVec);

        rBody.EmplaceCollider<Anwill::QuadCollider>(1.0f, 1.0f);
    });
}

void DynamicsTest::KillObjects()
{
    Anwill::Ecs::ForEach<Anwill::RBody>([](Anwill::EntityID id,
                    Anwill::RBody& rBody){
        if ( (rBody.GetPosition().Y <= -10.0f) or
             (rBody.GetPosition().X >= 1500.0f) )
        {
            Anwill::Ecs::RemoveEntity(id);
        }
    });

    struct ColEntity {
        Anwill::EntityID id;
        Anwill::Math::Mat4f transform;
        Anwill::RBody rBody;
    };

    std::vector<ColEntity> cols;
    std::set<Anwill::EntityID> toRemove;
    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&cols]
    (Anwill::EntityID id, Anwill::Math::Mat4f& transform, Anwill::RBody& rBody) {
        cols.emplace_back(id, transform, rBody);
    });

    for(unsigned int i = 0; i < cols.size(); i++)
    {
        if(toRemove.contains(cols[i].id)) {
            continue;
        }
        bool collidedOnce = false;
        for (unsigned int j = i + 1; j < cols.size(); j++)
        {
            if(toRemove.contains(cols[j].id)) {
                continue;
            }
            Anwill::CollisionData colData;
            if (Anwill::Collision::Check(cols[i].rBody, cols[i].transform,
                                         cols[j].rBody, cols[j].transform,
                                         colData))
            {
                toRemove.insert(cols[j].id);
                collidedOnce = true;
            }
        }
        if (collidedOnce)
        {
            toRemove.insert(cols[i].id);
        }
    }

    for(auto it = toRemove.begin(); it != toRemove.end(); it++)
    {
        Anwill::Ecs::RemoveEntity(*it);
    }
}
