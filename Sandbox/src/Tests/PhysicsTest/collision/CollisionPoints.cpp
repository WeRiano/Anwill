#include <set>

#include "CollisionPoints.h"

CollisionPoints::CollisionPoints(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_IsRound(false), m_Player(Anwill::Ecs::CreateEntity()), m_Camera(ws.width, ws.height)
{
    m_RectShader = Anwill::Shader::Create("assets/shaders/HelloUniform.glsl");

    m_CircleShader = Anwill::Shader::Create("assets/shaders/Circle.glsl");
    m_CircleShader->Bind();
    m_CircleShader->SetUniformVec1f(40.0f, "u_Radius");
    m_CircleShader->SetUniformVec3f(m_Camera.GetPos(), "u_CamPos");
    m_CircleShader->SetUniformVec3f(Anwill::Math::Vec3f(0.905f, 0.294f, 0.301f), "u_Color");
    m_CircleShader->Unbind();

    m_Mesh = Anwill::Mesh::CreateRectMesh(80.0f, 80.0f);

    Anwill::Ecs::RegisterComponent<Anwill::RBody>(); // TODO: Move to engine(?)
    Anwill::Ecs::RegisterComponent<Anwill::Math::Mat4f>();

    Anwill::Ecs::AddComponent<Anwill::RBody>(m_Player, 5.0f, Anwill::Math::Vec3f(400.0f, 400.0f, 0.0f));
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(m_Player);

    auto npc1 = Anwill::Ecs::CreateEntity();
    auto npc2 = Anwill::Ecs::CreateEntity();

    Anwill::Ecs::AddComponent<Anwill::RBody>(npc1, 5.0f, Anwill::Math::Vec3f(800.0f, 600.0f, 0.0f));
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc1);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc2, 5.0f, Anwill::Math::Vec3f(800.0f, 300.0f, 0.0f));
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc2);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::Math::Mat4f>([this](Anwill::EntityID id, Anwill::RBody& body,
            Anwill::Math::Mat4f& transform){
        auto vs = m_Mesh.GetVertices();
        body.SetCollider<Anwill::RectangleCollider>(Anwill::Math::Vec2f(vs[0].GetX(), vs[0].GetY()),
                                                     Anwill::Math::Vec2f(vs[1].GetX(), vs[1].GetY()),
                                                     Anwill::Math::Vec2f(vs[2].GetX(), vs[2].GetY()),
                                                     Anwill::Math::Vec2f(vs[3].GetX(), vs[3].GetY()));
    });

    Anwill::Renderer::SetClearColor({0.278f, 0.333f, 0.443f});
}

void CollisionPoints::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([this](Anwill::EntityID id,
                                                                       Anwill::Math::Mat4f& transform,
                                                                       Anwill::RBody& body) {
        auto pos = body.GetPosition();
        transform.SetTranslateCol({0.0f, 0.0f, 0.0f});
        //transform = Anwill::Math::Mat4f::RotateZ(transform, spinAngle);
        transform.SetTranslateCol(pos);

        if(m_IsRound)
        {
            Anwill::Renderer::Submit(m_CircleShader, m_Mesh, transform);
        } else {
            Anwill::Renderer::Submit(m_RectShader, m_Mesh, transform);
        }

        if (id == m_Player)
        {
            MovePlayer(body, transform);
        }
    });

    SwapShape();
    HandleCollisions();

    Layer::Update(timestamp);
}

void CollisionPoints::MovePlayer(Anwill::RBody& pBody, Anwill::Math::Mat4f& transform)
{
    float speed = 0.1f;
    if(Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
        pBody.Move({0.0f, speed, 0.0f});
    }
    if(Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
        pBody.Move({-speed, 0.0f, 0.0f});
    }
    if(Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
        pBody.Move({0.0f, -speed, 0.0f});
    }
    if(Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
        pBody.Move({speed, 0.0f, 0.0f});
    }
    if(Anwill::Input::IsKeyPressed(Anwill::KeyCode::Q)) {
        transform = Anwill::Math::Mat4f::RotateZ(transform, 0.05f);
    }
    if(Anwill::Input::IsKeyPressed(Anwill::KeyCode::E)) {
        transform = Anwill::Math::Mat4f::RotateZ(transform, -0.05f);
    }
}

void CollisionPoints::HandleCollisions()
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
                    if (Anwill::Collision::CheckCollision(bodyA, transformA,
                                                          bodyB, transformB, data))
                    {
                        Anwill::Collision::ResolveCollision(bodyA, transformA, bodyB, transformB, data);
                    }
                    colSet.insert(std::pair<Anwill::EntityID, Anwill::EntityID>(idA, idB));
                }
            }
        });
    });
}

void CollisionPoints::SwapShape()
{
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::C))
    {
        Anwill::Ecs::ForEach<Anwill::RBody>([](Anwill::EntityID id, Anwill::RBody& body) {
            body.SetCollider<Anwill::CircleCollider>(40.0f);
        });
        m_IsRound = true;
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::R))
    {
        Anwill::Ecs::ForEach<Anwill::RBody>([this](Anwill::EntityID id, Anwill::RBody& body) {
            auto vs = m_Mesh.GetVertices();
            body.SetCollider<Anwill::RectangleCollider>(Anwill::Math::Vec2f(vs[0].GetX(), vs[0].GetY()),
                                                        Anwill::Math::Vec2f(vs[1].GetX(), vs[1].GetY()),
                                                        Anwill::Math::Vec2f(vs[2].GetX(), vs[2].GetY()),
                                                        Anwill::Math::Vec2f(vs[3].GetX(), vs[3].GetY()));
        });
        m_IsRound = false;
    }
}