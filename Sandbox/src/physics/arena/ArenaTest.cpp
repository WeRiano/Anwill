#include "ArenaTest.h"

ArenaTest::ArenaTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_WS(ws)
{}

void ArenaTest::Update(const Anwill::Timestamp& timestamp)
{
    MoveAndTiltPlayer();
    HandleCollisions();
    SwapShape();
    WrapBodies();

    Layer::Update(timestamp);
}

void ArenaTest::HandleCollisions()
{
    std::set<std::pair<Anwill::EntityID, Anwill::EntityID>> colSet;
    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&colSet](Anwill::EntityID idA,
                                                                       Anwill::Math::Mat4f& transformA,
                                                                       Anwill::RBody& bodyA) {
        Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([&idA, &transformA, &bodyA, &colSet](Anwill::EntityID idB,
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
                        Anwill::Collision::Resolve(bodyA, transformA, bodyB, transformB,
                                                   data);
                    }
                    colSet.insert(std::pair<Anwill::EntityID, Anwill::EntityID>(idA, idB));
                }
            }
        });
    });
}

void ArenaTest::SwapShape()
{
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::C))
    {
        Anwill::Ecs::ForEntity<Anwill::RBody>(ArenaRender::s_Player, [](Anwill::RBody& body) {
            body.EmplaceCollider<Anwill::CircleCollider>(40.0f);
        });
        ArenaRender::s_PlayerIsRound = true;
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::R))
    {
        Anwill::Ecs::ForEntity<Anwill::RBody>(ArenaRender::s_Player, [this](Anwill::RBody& body) {
            auto vs = ArenaRender::s_Mesh.GetVertices();
            body.EmplaceCollider<Anwill::PolygonCollider>(vs);
        });
        ArenaRender::s_PlayerIsRound = false;
    }
}

void ArenaTest::MoveAndTiltPlayer()
{
    Anwill::Ecs::ForEntity<Anwill::Math::Mat4f, Anwill::RBody>(ArenaRender::s_Player, [](Anwill::Math::Mat4f& transform,
                                                                                         Anwill::RBody& pBody) {
        float speed = 0.1f;
        float velMag = 100.0f;
        Anwill::Math::Vec3f newVel = {};
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W))
        {
            newVel += {0.0f, velMag, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A))
        {
            newVel += {-velMag, 0.0f, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S))
        {
            newVel += {0.0f, -velMag, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D))
        {
            newVel += {velMag, 0.0f, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Space))
        {
            newVel = Anwill::Math::Vec3f();
            pBody.SetVelocity({});
        }
        pBody.ApplyImpulse(newVel);

        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Q))
        {
            transform = Anwill::Math::Mat4f::RotateZ(transform, 1.5f);
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::E))
        {
            transform = Anwill::Math::Mat4f::RotateZ(transform, -1.5f);
        }
    });
}

void ArenaTest::WrapBodies()
{
    Anwill::Ecs::ForEach<Anwill::RBody>([this](Anwill::EntityID id, Anwill::RBody& body){
        // We are assuming camera positioning at origin here
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
