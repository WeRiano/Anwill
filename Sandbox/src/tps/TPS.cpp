#include "TPS.h"
#include "TPSCalcs.h"

TPS::TPS(const unsigned int ups, float chunkWidth, float chunkHeight,
         unsigned int worldWidth, unsigned int worldHeight,
         const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height),
    m_PlayerTransform(Anwill::Math::Mat4f::Identity())
{
    AW_INFO("TPS Window Size: {0}, {1}", ws.width, ws.height);

    float boardWidth = chunkWidth * (float) worldWidth;
    float boardHeight = chunkHeight * (float) worldHeight;

    AW_INFO("TPS Board Size: {0}, {1}", boardWidth, boardHeight);

    m_WorldMesh = Anwill::Mesh::CreateRectMesh(boardWidth,boardHeight);
    m_PlayerMesh = Anwill::Mesh::CreateRectMesh(chunkWidth * 0.75f, chunkHeight * 0.75f);

    float deadSpaceWidth = ws.width - boardWidth;
    float deadSpaceHeight = ws.height - boardHeight;
    m_Camera.Move(-((boardWidth / 2.0f) + deadSpaceWidth / 2.0f),
                  -((boardHeight / 2.0f) + deadSpaceHeight / 2.0f));

    m_ShadowShader = Anwill::Shader::Create("Sandbox/assets/shaders/TPS.glsl");
    m_PlayerShader = Anwill::Shader::Create("Sandbox/assets/shaders/Ellipse.glsl");

    m_PlayerShader->Bind();
    m_PlayerShader->SetUniform1f(20.0f, "u_Radius");
    m_PlayerShader->SetUniformVec3f(m_Camera.GetPos(), "u_CamPos");
    m_PlayerShader->SetUniformVec3f(Anwill::Math::Vec3f(0.3f, 0.5f, 0.7f), "u_Color");
    m_PlayerShader->Unbind();

    m_ShadowShader->Bind();
    m_ShadowShader->SetUniformVec3f(m_Camera.GetPos(), "u_CamPos");
    m_ShadowShader->SetUniform1f(35.0f, "u_FoVAngleDeg");
    m_ShadowShader->Unbind();

    unsigned int eID = Anwill::Ecs::CreateEntity();
    Anwill::Ecs::AddComponent<EntityComponent>(eID);
    Anwill::Ecs::ForEach<EntityComponent>([this](Anwill::EntityID id,
            EntityComponent& comp) {
        comp.camPos = m_Camera.GetPos();
    });

    CreateWallSegments();
}

void TPS::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);

    Anwill::Renderer2D::BeginScene(m_Camera);

    Anwill::Ecs::ForEach<EntityComponent>([this](Anwill::EntityID id,
            EntityComponent& comp) {
        m_PlayerTransform.SetTranslation(comp.playerPos);

        m_ShadowShader->Bind();
        m_ShadowShader->SetUniformVec2f(comp.dir, "u_LookDir");
        m_ShadowShader->SetUniformVec2f(comp.playerPos, "u_PlayerPos");
        m_ShadowShader->Unbind();
    });

    Anwill::Renderer2D::SubmitMesh(m_ShadowShader, m_WorldMesh,
                                   Anwill::Math::Mat4f::Identity());
    Anwill::Renderer2D::SubmitMesh(m_PlayerShader, m_PlayerMesh, m_PlayerTransform);
}

void TPS::CreateWallSegments()
{
    // Board is from
    std::vector<Anwill::Math::Vec2f> segments;
    segments.push_back({-200.0f, 300});
    segments.push_back({0.0f, 300});
    segments.push_back({0.0f, 300});
    segments.push_back({50.0f, -100});

    for(unsigned int i = 0; i < segments.size(); i += 2) {
        std::string name1 = "u_WallSegs[" + std::to_string(i) + "].p1";
        std::string name2 = "u_WallSegs[" + std::to_string(i) + "].p2";
        m_ShadowShader->Bind();
        m_ShadowShader->SetUniformVec2f(segments[i], name1);
        m_ShadowShader->SetUniformVec2f(segments[i+1], name2);
        m_ShadowShader->Unbind();
    }
}
