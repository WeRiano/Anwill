#include <set>

#include "ArenaRender.h"

bool ArenaRender::s_PlayerIsRound = false;
Anwill::EntityID ArenaRender::s_Player;
Anwill::Mesh ArenaRender::s_Mesh;

ArenaRender::ArenaRender(unsigned int ups, const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height)
{
    s_Mesh = Anwill::Mesh::CreateRectMesh(80.0f, 80.0f);

    m_RectShader = Anwill::Shader::Create("Sandbox/assets/shaders/RectBorder.glsl");
    m_RectShader->Bind();
    m_RectShader->SetUniformVec2f(Anwill::Math::Vec2f(80.0f, 80.0f), "u_WidthHeight");
    m_RectShader->Unbind();

    m_CircleShader = Anwill::Shader::Create("Sandbox/assets/shaders/Ellipse.glsl");
    m_CircleShader->Bind();
    m_CircleShader->SetUniform1f(40.0f, "u_Radius");
    m_CircleShader->SetUniformVec3f(m_Camera.GetPos(), "u_CamPos");
    m_CircleShader->SetUniformVec3f(Anwill::Math::Vec3f(0.905f, 0.294f, 0.301f), "u_Color");
    m_CircleShader->Unbind();

    m_Camera.Move(ws.width * 0.5f, ws.height * 0.5f);

    Anwill::Ecs::RegisterComponent<Anwill::RBody>(); // TODO: Move to engine(?)
    Anwill::Ecs::RegisterComponent<Anwill::Math::Mat4f>();

    float mass = 5.0f;

    // Even ids are circles
    Anwill::EntityID npc1 = Anwill::Ecs::CreateEntity(); // Rect
    Anwill::EntityID npc2 = Anwill::Ecs::CreateEntity(); // Ellipse
    Anwill::EntityID npc3 = Anwill::Ecs::CreateEntity(); // Rect
    Anwill::EntityID npc4 = Anwill::Ecs::CreateEntity(); // Ellipse

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
            body.EmplaceCollider<Anwill::CircleCollider>(40.0f);
        } else
        {
            auto vs = s_Mesh.GetVertices();
            body.EmplaceCollider<Anwill::PolygonCollider>(vs);
        }
    });

    Anwill::Renderer::SetClearColor({0.278f, 0.333f, 0.443f});
}

void ArenaRender::Update(const Anwill::Timestamp& timestamp)
{
    float delta = GetUpdateDelta(timestamp).GetSeconds();

    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<Anwill::Math::Mat4f, Anwill::RBody>([this, delta](Anwill::EntityID id,
                                                                       Anwill::Math::Mat4f& transform,
                                                                       Anwill::RBody& body) {

        Anwill::Math::Vec3f pos = body.GetPosition();
        transform.SetTranslation(pos);

        if(id == s_Player)
        {
            if(s_PlayerIsRound) {
                Anwill::Renderer2D::PushCircleToBatch(transform, {1.0f, 0.0f, 0.0f});
            } else {
                Anwill::Renderer2D::PushQuadToBatch(transform, {0.858f, 0.552f, 0.223f});
            }
        } else {
            if(id % 2 == 0)
            {
                Anwill::Renderer2D::PushCircleToBatch(transform, {1.0f, 1.0f, 0.0f});
            } else {
                Anwill::Renderer2D::PushQuadToBatch(transform, {0.258f, 0.852f, 0.223f});
            }
        }

        auto vel = body.GetVelocity();
        vel.Normalize();
        body.ApplyForce(-vel * 250.0f);
        body.Tick(delta * 2.0f);

        Anwill::Renderer2D::DrawBatch();
    });

    Layer::Update(timestamp);
}
