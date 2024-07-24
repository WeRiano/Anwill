#include "ArenaTest.h"

bool ArenaTest::s_PlayerIsRound = false;
Anwill::EntityID ArenaTest::s_Player;

ArenaTest::ArenaTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_WS(ws), m_VelocityMagnitude(50), m_N(250.0f), m_CoR(0.8f),
      m_TimeMultiplier(2.0f), m_WrapBodies(true), m_NumCollisions(0)
{
    Anwill::Ecs::Reset();

    Anwill::Ecs::RegisterComponent<Anwill::RBody>();
    Anwill::Ecs::RegisterComponent<Anwill::Math::Mat4f>();

    // Even ids are circles
    Anwill::EntityID npc1 = Anwill::Ecs::CreateEntity();
    Anwill::EntityID npc2 = Anwill::Ecs::CreateEntity();
    Anwill::EntityID npc3 = Anwill::Ecs::CreateEntity();
    Anwill::EntityID npc4 = Anwill::Ecs::CreateEntity();
    Anwill::EntityID npc5 = Anwill::Ecs::CreateEntity();

    s_Player = Anwill::Ecs::CreateEntity();

    auto objTransform = Anwill::Math::Mat4f::Scale({}, {80.0f, 80.0f, 0.0f});
    float objMass = 5.0f;

    Anwill::Ecs::AddComponent<Anwill::RBody>(s_Player, objMass, false,
                                             Anwill::Math::Vec3f(400.0f, 400.0f, 0.0f),
                                             Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                                             Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(s_Player, objTransform);

    Anwill::Ecs::AddComponent<Anwill::RBody>(npc1, objMass, false,
                                             Anwill::Math::Vec3f(800.0f, 600.0f, 0.0f),
                                             Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                                             Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc1, objTransform);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc2, objMass, false,
                                             Anwill::Math::Vec3f(800.0f, 300.0f, 0.0f),
                                             Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                                             Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc2, objTransform);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc3, objMass, false,
                                             Anwill::Math::Vec3f(200.0f, 600.0f, 0.0f),
                                             Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                                             Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc3, objTransform);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc4, objMass, false,
                                             Anwill::Math::Vec3f(200.0f, 300.0f, 0.0f),
                                             Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                                             Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc4, objTransform);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc5, objMass, true,
                                             Anwill::Math::Vec3f(ws.width * 0.5f, ws.height * 0.5f, 0.0f),
                                             Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                                             Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc5, objTransform);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::Math::Mat4f>([](Anwill::EntityID id, Anwill::RBody& body,
                                                                Anwill::Math::Mat4f& transform){
        if(id % 2 == 0 and id != s_Player) {
            body.EmplaceCollider<Anwill::CircleCollider>(0.5f);
        } else {
            auto vs = Anwill::Mesh::GetUnitRectangle(false).GetVertices();
            body.EmplaceCollider<Anwill::PolygonCollider>(vs);
        }
    });

    Anwill::Renderer::SetClearColor({0.278f, 0.333f, 0.443f});
}

void ArenaTest::Update(const Anwill::Timestamp& timestamp)
{
    MoveAndRotatePlayer();
    HandleCollisions();
    TickBodies(GetUpdateDelta(timestamp));
    if(m_WrapBodies) {
        WrapBodies(GetUpdateDelta(timestamp));
    }

    Layer::Update(timestamp);
}

void ArenaTest::ImguiUpdate()
{
    ImGui::SetNextWindowBgAlpha(0.40f);
    ImGui::Begin("Arena");

    static int shape = 0;
    Anwill::Ecs::ForEntity<Anwill::RBody>(s_Player, [this](Anwill::RBody& body) {
        ImGui::Text("Collisions: %d", m_NumCollisions);
        ImGui::Text("Player velocity (magnitude): %.1f", body.GetVelocity().GetLength());

        ImGui::Text("Player shape"); ImGui::SameLine();
        if(ImGui::RadioButton("Quad", &shape, 0)) {
            body.EmplaceCollider<Anwill::PolygonCollider>(Anwill::Mesh::GetUnitRectangle(false).GetVertices());
        }  ImGui::SameLine();
        if (ImGui::RadioButton("Circle", &shape, 1)) {
            body.EmplaceCollider<Anwill::CircleCollider>(0.5f);
        }
    });
    s_PlayerIsRound = (bool)shape;

    ImGui::SliderFloat("Movement force", &m_VelocityMagnitude, 1.0f, 200.0f);
    ImGui::SliderFloat("Friction normal force", &m_N, 0.0f, 1000.0f);
    ImGui::SliderFloat("Coefficient of restitution", &m_CoR, 0.0f, 1.0f);
    ImGui::SliderFloat("Time multiplier", &m_TimeMultiplier, 1.0f, 10.0f);
    if (ImGui::Checkbox("Wrap (teleport) entities", &m_WrapBodies)) {
        (m_WrapBodies) ? KillWalls() : SpawnWalls();
    }
    ImGui::End();
}

void ArenaTest::HandleCollisions()
{
    std::set<std::pair<Anwill::EntityID, Anwill::EntityID>> colSet;
    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&colSet, this](Anwill::EntityID idA,
                                                                       Anwill::Math::Mat4f& transformA,
                                                                       Anwill::RBody& bodyA) {
        Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>(
                [&idA, &transformA, &bodyA, &colSet, this](Anwill::EntityID idB,
                        Anwill::Math::Mat4f& transformB, Anwill::RBody& bodyB) {
            if(idA != idB && !(bodyA.IsStatic() && bodyB.IsStatic()))
            {
                if(!colSet.contains(std::pair<Anwill::EntityID, Anwill::EntityID>(idB, idA)))
                {
                    Anwill::CollisionData data;
                    if (Anwill::Collision::Check(bodyA, transformA, bodyB, transformB, data))
                    {
                        m_NumCollisions++;
                        data.e = m_CoR;
                        Anwill::Collision::Resolve(bodyA, transformA, bodyB, transformB, data);
                    }
                    colSet.insert(std::pair<Anwill::EntityID, Anwill::EntityID>(idA, idB));
                }
            }
        });
    });
}

void ArenaTest::MoveAndRotatePlayer()
{
    Anwill::Ecs::ForEntity<Anwill::Math::Mat4f, Anwill::RBody>(s_Player,
                                                               [this](Anwill::Math::Mat4f& transform,
                                                                       Anwill::RBody& pBody) {
        Anwill::Math::Vec3f newVel = {};
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
            newVel += {0.0f, m_VelocityMagnitude, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
            newVel += {-m_VelocityMagnitude, 0.0f, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
            newVel += {0.0f, -m_VelocityMagnitude, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
            newVel += {m_VelocityMagnitude, 0.0f, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Space)) {
            newVel = Anwill::Math::Vec3f();
            pBody.SetVelocity({});
        }
        pBody.ApplyImpulse(newVel);

        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Q)) {
            transform = Anwill::Math::Mat4f::RotateZ(transform, 1.5f);
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::E)) {
            transform = Anwill::Math::Mat4f::RotateZ(transform, -1.5f);
        }
    });
}

void ArenaTest::WrapBodies(const Anwill::Timestamp delta)
{
    Anwill::Ecs::ForEach<Anwill::RBody>([this](Anwill::EntityID id, Anwill::RBody& body) {
        // We are assuming camera positioning at origin here
        if(body.IsStatic()) {
            return;
        }
        float margin = 10.0f;
        auto pos = body.GetPosition();
        if(pos.X < -margin) {
            body.Move({(float) m_WS.width + margin * 2, 0.0f, 0.0f});
        }
        if(pos.X > m_WS.width + margin) {
            body.Move({-(float) m_WS.width - margin * 2, 0.0f, 0.0f});
        }
        if(pos.Y < -margin) {
            body.Move({0.0f, (float) m_WS.height + margin * 2, 0.0f});
        }
        if(pos.Y > m_WS.height + margin) {
            body.Move({0.0f, -(float) m_WS.height - margin * 2, 0.0f});
        }
    });
}

void ArenaTest::TickBodies(Anwill::Timestamp delta)
{
    Anwill::Ecs::ForEach<Anwill::RBody>([this, delta](Anwill::EntityID id, Anwill::RBody& body){
        // We are assuming camera positioning at origin here
        if(body.IsStatic()) {
            return;
        }
        body.ApplyFriction(1.0f, m_N);
        body.Tick((float)delta.GetSeconds() * m_TimeMultiplier);
    });
}

void ArenaTest::SpawnWalls()
{
    m_Walls[0] = Anwill::Ecs::CreateEntity(); // North
    m_Walls[1] = Anwill::Ecs::CreateEntity(); // East
    m_Walls[2] = Anwill::Ecs::CreateEntity(); // South
    m_Walls[3] = Anwill::Ecs::CreateEntity(); // West

    float wallThickness = 50.0f;
    Anwill::Math::Vec3f northPos = Anwill::Math::Vec3f(m_WS.width * 0.5f, m_WS.height + wallThickness * 0.5f, 0.0f);
    Anwill::Math::Vec3f eastPos = Anwill::Math::Vec3f(m_WS.width + wallThickness * 0.5f, m_WS.height * 0.5f, 0.0f);
    Anwill::Math::Vec3f southPos = Anwill::Math::Vec3f(m_WS.width * 0.5f, -wallThickness * 0.5f, 0.0f);
    Anwill::Math::Vec3f westPos = Anwill::Math::Vec3f(-wallThickness * 0.5f, m_WS.height * 0.5f, 0.0f);

    Anwill::Math::Vec3f northScale =
            Anwill::Math::Vec3f((float)m_WS.width + (wallThickness * 2.0f), wallThickness, 0.0f);
    Anwill::Math::Vec3f eastScale =
            Anwill::Math::Vec3f(wallThickness, (float)m_WS.height - (wallThickness * 2.0f), 0.0f);
    Anwill::Math::Vec3f southScale =
            Anwill::Math::Vec3f((float)m_WS.width + (wallThickness * 2.0f), wallThickness, 0.0f);
    Anwill::Math::Vec3f westScale =
            Anwill::Math::Vec3f(wallThickness, (float)m_WS.height - (wallThickness * 2.0f), 0.0f);

    float wallMass = 100.0f;

    Anwill::RBody northBody(wallMass, true, northPos, Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                            Anwill::Math::Vec3f());
    Anwill::RBody eastBody(wallMass, true, eastPos, Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                           Anwill::Math::Vec3f());
    Anwill::RBody southBody(wallMass, true, southPos, Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                            Anwill::Math::Vec3f());
    Anwill::RBody westBody(wallMass, true, westPos, Anwill::Math::Vec3f(), Anwill::Math::Vec3f(),
                           Anwill::Math::Vec3f());

    northBody.EmplaceCollider<Anwill::PolygonCollider>(Anwill::Mesh::GetUnitRectangle(false).GetVertices());
    eastBody.EmplaceCollider<Anwill::PolygonCollider>(Anwill::Mesh::GetUnitRectangle(false).GetVertices());
    southBody.EmplaceCollider<Anwill::PolygonCollider>(Anwill::Mesh::GetUnitRectangle(false).GetVertices());
    westBody.EmplaceCollider<Anwill::PolygonCollider>(Anwill::Mesh::GetUnitRectangle(false).GetVertices());

    Anwill::Ecs::AddComponent<Anwill::RBody>(m_Walls[0], northBody);
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(m_Walls[0], Anwill::Math::Mat4f::Scale({}, northScale));

    Anwill::Ecs::AddComponent<Anwill::RBody>(m_Walls[1], eastBody);
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(m_Walls[1], Anwill::Math::Mat4f::Scale({}, eastScale));

    Anwill::Ecs::AddComponent<Anwill::RBody>(m_Walls[2], southBody);
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(m_Walls[2], Anwill::Math::Mat4f::Scale({}, southScale));

    Anwill::Ecs::AddComponent<Anwill::RBody>(m_Walls[3], westBody);
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(m_Walls[3], Anwill::Math::Mat4f::Scale({}, westScale));
}

void ArenaTest::KillWalls()
{
    Anwill::Ecs::RemoveEntity(m_Walls[0]);
    Anwill::Ecs::RemoveEntity(m_Walls[1]);
    Anwill::Ecs::RemoveEntity(m_Walls[2]);
    Anwill::Ecs::RemoveEntity(m_Walls[3]);
}
