#include <set>

#include "CollisionRender.h"

bool CollisionRender::s_PlayerIsRound = false;
Anwill::EntityID CollisionRender::s_Player;
Anwill::Mesh CollisionRender::s_Mesh;


CollisionRender::CollisionRender(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height)
{
    s_Mesh = Anwill::Mesh::CreateRectMesh(80.0f, 80.0f);

    m_RectShader = Anwill::Shader::Create("assets/shaders/RectBorder.glsl");
    m_RectShader->Bind();
    m_RectShader->SetUniformVec2f(Anwill::Math::Vec2f(80.0f, 80.0f), "u_WidthHeight");
    m_RectShader->Unbind();

    m_CircleShader = Anwill::Shader::Create("assets/shaders/Circle.glsl");
    m_CircleShader->Bind();
    m_CircleShader->SetUniform1f(40.0f, "u_Radius");
    m_CircleShader->SetUniformVec3f(m_Camera.GetPos(), "u_CamPos");
    m_CircleShader->SetUniformVec3f(Anwill::Math::Vec3f(0.905f, 0.294f, 0.301f), "u_Color");
    m_CircleShader->Unbind();

    Anwill::Ecs::RegisterComponent<Anwill::RBody>(); // TODO: Move to engine(?)
    Anwill::Ecs::RegisterComponent<Anwill::Math::Mat4f>();

    float mass = 5.0f;

    // Even are circles
    auto npc1 = Anwill::Ecs::CreateEntity(); // Rect
    auto npc2 = Anwill::Ecs::CreateEntity(); // Circle
    auto npc3 = Anwill::Ecs::CreateEntity(); // Rect
    auto npc4 = Anwill::Ecs::CreateEntity(); // Circle

    s_Player = Anwill::Ecs::CreateEntity();

    Anwill::Ecs::AddComponent<Anwill::RBody>(s_Player, mass, false, Anwill::Math::Vec3f(400.0f, 400.0f, 0.0f), Anwill::Math::Vec3f(), Anwill::Math::Vec3f(), Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(s_Player);

    Anwill::Ecs::AddComponent<Anwill::RBody>(npc1, mass, false, Anwill::Math::Vec3f(800.0f, 600.0f, 0.0f), Anwill::Math::Vec3f(), Anwill::Math::Vec3f(), Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc1);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc2, mass, false, Anwill::Math::Vec3f(800.0f, 300.0f, 0.0f), Anwill::Math::Vec3f(), Anwill::Math::Vec3f(), Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc2);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc3, mass, false, Anwill::Math::Vec3f(200.0f, 600.0f, 0.0f), Anwill::Math::Vec3f(), Anwill::Math::Vec3f(), Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc3);
    Anwill::Ecs::AddComponent<Anwill::RBody>(npc4, mass, true, Anwill::Math::Vec3f(200.0f, 300.0f, 0.0f), Anwill::Math::Vec3f(), Anwill::Math::Vec3f(), Anwill::Math::Vec3f());
    Anwill::Ecs::AddComponent<Anwill::Math::Mat4f>(npc4);

    Anwill::Ecs::ForEach<Anwill::RBody, Anwill::Math::Mat4f>([](Anwill::EntityID id, Anwill::RBody& body,
            Anwill::Math::Mat4f& transform){
        if(id % 2 == 0 and id != s_Player)
        {
            body.SetCollider<Anwill::CircleCollider>(40.0f);
        } else
        {
            auto vs = s_Mesh.GetVertices();
            body.SetCollider<Anwill::PolygonCollider>(vs);
        }
    });

    Anwill::Renderer::SetClearColor({0.278f, 0.333f, 0.443f});
}

void CollisionRender::Update(const Anwill::Timestamp& timestamp)
{
    float delta = timestamp.GetSeconds() - m_LastUpdate.GetSeconds();

    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([this, delta](Anwill::EntityID id,
                                                                       Anwill::Math::Mat4f& transform,
                                                                       Anwill::RBody& body) {
        bool isCircle = false;
        if ((id == s_Player and s_PlayerIsRound) or id % 2 == 0 ) {
            isCircle = true;
        }

        auto pos = body.GetPosition();
        //transform.SetTranslateCol({0.0f, 0.0f, 0.0f});
        //transform = Anwill::Math::Mat4f::RotateZ(transform, spinAngle);
        transform.SetTranslateCol(pos);

        if(id == s_Player)
        {
            if(s_PlayerIsRound) {
                Anwill::Renderer2D::Submit(m_CircleShader, s_Mesh, transform);
            } else {
                Anwill::Renderer2D::Submit(m_RectShader, s_Mesh, transform);
            }
        } else {
            if(id % 2 == 0)
            {
                Anwill::Renderer2D::Submit(m_CircleShader, s_Mesh, transform);
            } else {
                Anwill::Renderer2D::Submit(m_RectShader, s_Mesh, transform);
            }
        }

        auto vel = body.GetVelocity();
        vel.Normalize();
        body.ApplyForce(-vel * 250.0f);
        body.Tick(delta * 2.0f);
    });

    Layer::Update(timestamp);
}
