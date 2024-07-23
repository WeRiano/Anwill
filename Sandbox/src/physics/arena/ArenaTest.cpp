#include "ArenaTest.h"

ArenaTest::ArenaTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_WS(ws), m_VelocityMagnitude(50), m_FrictionMultiplier(250.0f), m_CoR(0.8f)
{}

void ArenaTest::Update(const Anwill::Timestamp& timestamp)
{
    MoveAndRotatePlayer();
    HandleCollisions();
    WrapAndSlowBodies(GetUpdateDelta(timestamp));

    Layer::Update(timestamp);
}

void ArenaTest::ImguiUpdate()
{
    ImGui::Begin("Arena");

    static int shape = 0;
    ImGui::SeparatorText("Player");
    Anwill::Ecs::ForEntity<Anwill::RBody>(ArenaRender::s_Player, [](Anwill::RBody& body) {
        if(ImGui::RadioButton("Quad", &shape, 0)) {
            auto vs = ArenaRender::s_Mesh.GetVertices();
            body.EmplaceCollider<Anwill::PolygonCollider>(vs);
        }  ImGui::SameLine();
        if (ImGui::RadioButton("Circle", &shape, 1)) {
            body.EmplaceCollider<Anwill::CircleCollider>(0.5f);
        }
        ImGui::Text("Velocity magnitude: %.1f", body.GetVelocity().GetLength());
    });
    ArenaRender::s_PlayerIsRound = (bool)shape;

    ImGui::SliderFloat("Movement force", &m_VelocityMagnitude, 1.0f, 200.0f);
    ImGui::SliderFloat("Friction multiplier", &m_FrictionMultiplier, 0.0f, 500.0f);
    ImGui::SliderFloat("Coefficient of restitution", &m_CoR, 0.0f, 1.0f);

    ImGui::End();
}

void ArenaTest::HandleCollisions()
{
    std::set<std::pair<Anwill::EntityID, Anwill::EntityID>> colSet;
    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&colSet, this](Anwill::EntityID idA,
                                                                       Anwill::Math::Mat4f& transformA,
                                                                       Anwill::RBody& bodyA) {
        Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&idA, &transformA, &bodyA, &colSet, this](Anwill::EntityID idB,
                                                                                                      Anwill::Math::Mat4f& transformB,
                                                                                                      Anwill::RBody& bodyB) {
            if(idA != idB)
            {
                if(!colSet.contains(std::pair<Anwill::EntityID, Anwill::EntityID>(idB, idA)))
                {
                    Anwill::CollisionData data;
                    if (Anwill::Collision::Check(bodyA, transformA,
                                                 bodyB, transformB, data))
                    {
                        data.e = m_CoR;
                        Anwill::Collision::Resolve(bodyA, transformA, bodyB, transformB,
                                                   data);
                    }
                    colSet.insert(std::pair<Anwill::EntityID, Anwill::EntityID>(idA, idB));
                }
            }
        });
    });
}

void ArenaTest::MoveAndRotatePlayer()
{
    Anwill::Ecs::ForEntity<Anwill::Math::Mat4f, Anwill::RBody>(ArenaRender::s_Player, [this](Anwill::Math::Mat4f& transform,
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

void ArenaTest::WrapAndSlowBodies(const Anwill::Timestamp delta)
{
    Anwill::Ecs::ForEach<Anwill::RBody>([this, delta](Anwill::EntityID id, Anwill::RBody& body){
        // We are assuming camera positioning at origin here
        float margin = 10.0f;

        auto vel = body.GetVelocity();
        vel.Normalize();
        body.ApplyForce(-vel * m_FrictionMultiplier); // TODO: Apply friction "properly"
        body.Tick((float)delta.GetSeconds() * 2.0f);

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
