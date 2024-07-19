#include "SprinklerTest.h"

SprinklerTest::SprinklerTest(unsigned int ups)
    : Anwill::Layer(ups),
      m_SpawnTimeCount(1000000 * s_SpawnDeltaSeconds),
      m_ObjSize(20.0f)
{
    Anwill::Ecs::RegisterComponent<Anwill::RBody>();
    Anwill::Ecs::RegisterComponent<Anwill::Math::Mat4f>();
    Anwill::Ecs::RegisterComponent<Anwill::Mesh>();
}

void SprinklerTest::Update(const Anwill::Timestamp& timestamp)
{
    // Increment a counter and spawn an object when the counter has reached some value,
    // then reset counter.
    m_SpawnTimeCount += GetUpdateDelta(timestamp);
    if (m_SpawnTimeCount.GetSeconds() >= s_SpawnDeltaSeconds) {
        m_SpawnTimeCount = Anwill::Timestamp(0);
        SpawnObject();
    }

    // Cleanup all "dead" objects.
    KillObjects();

    Layer::Update(timestamp);
}

void SprinklerTest::ImguiUpdate()
{
    ImGui::Begin("Sprinkler");

    ImGui::SliderFloat("Particle size", &m_ObjSize, 10.0f, 40.0f);

    Anwill::EcsStats ecsStats = Anwill::Ecs::GetStats();
    ImGui::SeparatorText("ECS");
    ImGui::Text("Registered component types: %d", ecsStats.numRegisteredComponents);
    ImGui::Text("Alive entities: %d", ecsStats.numAliveEntities);
    ImGui::Text("Alive components: %d", ecsStats.numComponents);

    ImGui::End();
}

void SprinklerTest::SpawnObject()
{
    AW_PROFILE_FUNC();

    Anwill::EntityID newID = Anwill::Ecs::CreateEntity();
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(newID,
                                                   Anwill::Math::Mat4f::Scale({},
                                                                              {m_ObjSize, m_ObjSize                                                                                           , 0.0f}));
    Anwill::Ecs::AddComponent<Anwill::RBody>(newID, 2.5f, false);

    Anwill::Ecs::ForEntity<Anwill::RBody>(newID, [](Anwill::RBody& rBody) {
        float randAngle = Anwill::Random::GetUniformFloat(-20.0f, 20.0f);
        auto pushVec = Anwill::Math::Vec3f(150.0f, 350.0f, 0.0f);
        auto rotMat = Anwill::Math::Mat4f::RotateZ(Anwill::Math::Mat4f::Identity(), randAngle);
        pushVec = rotMat * pushVec;
        rBody.ApplyImpulse(pushVec);

        rBody.EmplaceCollider<Anwill::QuadCollider>(1.0f, 1.0f);
    });
}

void SprinklerTest::KillObjects()
{
    AW_PROFILE_FUNC();

    // Remove objects that are in Africa
    Anwill::Ecs::ForEach<Anwill::RBody>([](Anwill::EntityID id, Anwill::RBody& rBody) {
        if ( (rBody.GetPosition().Y <= -10.0f) or (rBody.GetPosition().X >= 3000.0f) ) {
            Anwill::Ecs::RemoveEntity(id);
        }
    });

    struct ColEntity {
        Anwill::EntityID id;
        Anwill::Math::Mat4f transform;
        Anwill::RBody rBody;
    };

    // Grab all entities
    std::vector<ColEntity> cols;
    std::set<Anwill::EntityID> toRemove;
    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&cols]
    (Anwill::EntityID id, Anwill::Math::Mat4f& transform, Anwill::RBody& rBody) {
        cols.emplace_back(id, transform, rBody);
    });

    // Loop through all entities (O(n^2)) and grab colliding entities
    for(unsigned int i = 0; i < cols.size(); i++) {
        if(toRemove.contains(cols[i].id)) {
            continue;
        }
        for (unsigned int j = i + 1; j < cols.size(); j++)
        {
            if(toRemove.contains(cols[j].id)) {
                continue;
            }
            Anwill::CollisionData colData;
            if (Anwill::Collision::Check(cols[i].rBody, cols[i].transform,
                                         cols[j].rBody, cols[j].transform,
                                         colData)) {
                toRemove.insert(cols[i].id);
                toRemove.insert(cols[j].id);
                break;
            }
        }
    }

    for(auto it = toRemove.begin(); it != toRemove.end(); it++)
    {
        Anwill::Ecs::RemoveEntity(*it);
    }
}
