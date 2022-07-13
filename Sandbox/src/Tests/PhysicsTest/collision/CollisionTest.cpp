#include "CollisionTest.h"

CollisionTest::CollisionTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_WS(ws)
{}

void CollisionTest::Update(const Anwill::Timestamp& timestamp)
{
    MoveAndTiltPlayer();
    HandleCollisions();
    SwapShape();
    WrapBodies();

    Layer::Update(timestamp);
}

void CollisionTest::HandleCollisions()
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
                    if (Anwill::CollisionTest::CheckCollision(bodyA, transformA,
                                                              bodyB, transformB, data))
                    {
                        Anwill::CollisionTest::ResolveCollision(bodyA, transformA, bodyB, transformB, data);
                    }
                    colSet.insert(std::pair<Anwill::EntityID, Anwill::EntityID>(idA, idB));
                }
            }
        });
    });
}

void CollisionTest::SwapShape()
{
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::C))
    {
        Anwill::Ecs::ForEntity<Anwill::RBody>(CollisionRender::s_Player, [](Anwill::RBody& body) {
            body.SetCollider<Anwill::CircleCollider>(40.0f);
        });
        CollisionRender::s_IsRound = true;
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::R))
    {
        Anwill::Ecs::ForEntity<Anwill::RBody>(CollisionRender::s_Player, [this](Anwill::RBody& body) {
            auto vs = CollisionRender::s_Mesh.GetVertices();
            body.SetCollider<Anwill::PolygonCollider>(vs);
        });
        CollisionRender::s_IsRound = false;
    }
}

void CollisionTest::MoveAndTiltPlayer()
{
    Anwill::Ecs::ForEntity<Anwill::Math::Mat4f, Anwill::RBody>(CollisionRender::s_Player, [](Anwill::Math::Mat4f& transform,
            Anwill::RBody& pBody) {
        float speed = 0.1f;
        float velMag = 100.0f;
        Anwill::Math::Vec3f newVel = {};
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W) or Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)
            or Anwill::Input::IsKeyPressed(Anwill::KeyCode::S) or Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W))
        {
            //pBody.Move({0.0f, speed, 0.0f});
            newVel += {0.0f, velMag, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A))
        {
            //pBody.Move({-speed, 0.0f, 0.0f});
            newVel += {-velMag, 0.0f, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S))
        {
            //pBody.Move({0.0f, -speed, 0.0f});
            newVel += {0.0f, -velMag, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D))
        {
            //pBody.Move({speed, 0.0f, 0.0f});
            newVel += {velMag, 0.0f, 0.0f};
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Space))
        {
            newVel = {};
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

void CollisionTest::WrapBodies()
{
    Anwill::Ecs::ForEach<Anwill::RBody>([this](Anwill::EntityID id, Anwill::RBody& body){
        // We are assuming camera positioning at origin here
        float margin = 10.0f;

        auto pos = body.GetPosition();
        if(pos.GetX() < -margin) {
            body.Move({(float) m_WS.width + margin * 2, 0.0f, 0.0f});
        }
        if(pos.GetX() > m_WS.width + margin) {
            body.Move({-(float) m_WS.width - margin * 2, 0.0f, 0.0f});
        }
        if(pos.GetY() < -margin) {
            body.Move({0.0f, (float) m_WS.height + margin * 2, 0.0f});
        }
        if(pos.GetY() > m_WS.height + margin) {
            body.Move({0.0f, -(float) m_WS.height - margin * 2, 0.0f});
        }
    });
}
