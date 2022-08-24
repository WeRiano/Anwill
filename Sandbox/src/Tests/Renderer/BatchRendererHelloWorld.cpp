#include "BatchRendererHelloWorld.h"

BatchRendererHelloWorld::BatchRendererHelloWorld(unsigned int ups,
                                                 const Anwill::WindowSettings& ws)
    : Anwill::Layer(ups), m_Camera(ws.width, ws.height), m_IsBatchRendering(true),
      m_SpriteAnimation(Anwill::Timestamp(1  * 1000000))
{
    m_QuadShader = Anwill::Shader::Create("assets/shaders/QuadBatch.glsl");
    m_SlowTextShader = Anwill::Shader::Create("assets/shaders/RectTexture.glsl");
    m_SlowColorShader = Anwill::Shader::Create("assets/shaders/RectColor.glsl");

    auto spriteSheet = Anwill::SpriteSheet::Create(
            "assets/textures/test_sprite_sheet.png", 64, 48);
    m_Sprite = std::make_unique<Anwill::Sprite>(spriteSheet, 3, 2, -1, 0, 0, -1);

    // TODO: Could implement like a range function or something to avoid clutter.
    // Add all sprites horizontally, vertically, or even diagonally for some range
    m_SpriteAnimation.AddFramesHorizontally(spriteSheet, 43, 48, 1, 10);

    m_TestTexture = Anwill::Texture::Create("assets/textures/awesomeface.png");

    m_QuadTextMesh = Anwill::Mesh::CreateRectMesh(1.0f, 1.0f, true);
    m_QuadColorMesh = Anwill::Mesh::CreateRectMesh(1.0f, 1.0f, false);

    m_QuadWidth = 20.0f;
    m_QuadHeight = 20.0f;
    m_CanvasWidth = ws.width * 2;
    m_CanvasHeight = ws.height * 2;
    m_NrQuadsX = static_cast<unsigned int>(m_CanvasWidth) /
            static_cast<unsigned int>(m_QuadWidth);
    m_NrQuadsY = static_cast<unsigned int>(m_CanvasHeight) /
            static_cast<unsigned int>(m_QuadHeight);

}

void BatchRendererHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);
    m_SpriteAnimation.Tick(GetUpdateDelta(timestamp));

    if(m_IsBatchRendering) {
        BatchRendering();
    } else {
        SlowRendering();
    }

    MoveCamera();

    Layer::Update(timestamp);
}

void BatchRendererHelloWorld::BatchRendering()
{
    AW_PROFILE_FUNC();
    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {m_QuadWidth, m_QuadHeight, 0.0f});
    //transform = Anwill::Math::Mat4f::Translate(transform, {-2000.0f, -2000.0f, 0.0f});
    for(unsigned int y = 0; y < m_NrQuadsY; y++)
    {
        for(unsigned int x = 0; x < m_NrQuadsX; x++)
        {
            float xClamp = Anwill::Utils::NormalizeBetween0And1(x, 0u, m_NrQuadsY);
            float yClamp = Anwill::Utils::NormalizeBetween0And1(y, 0u, m_NrQuadsX);
            Anwill::Math::Vec3f color(xClamp, yClamp, 1.0f);
            Anwill::Renderer2D::PushQuadToBatch(transform, m_QuadShader, color);
            transform = Anwill::Math::Mat4f::Translate(transform,
                                                       {m_QuadWidth, 0.0f, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {-static_cast<float>(m_CanvasWidth),
                                                    m_QuadHeight, 0.0f});
    }

    transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                           {m_QuadWidth * 10,
                                            m_QuadHeight * 10, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform,
                                               {static_cast<float>(m_CanvasWidth / 2),
                                                static_cast<float>(m_CanvasHeight / 2),
                                                0.0f});
    Anwill::Renderer2D::PushQuadToBatch(transform, m_QuadShader, m_SpriteAnimation);
    Anwill::Renderer2D::DrawBatch(m_QuadShader);
}

void BatchRendererHelloWorld::SlowRendering()
{
    AW_PROFILE_FUNC();
    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {m_QuadWidth, m_QuadHeight, 0.0f});
    for(unsigned int y = 0; y < m_NrQuadsY; y++)
    {
        for(unsigned int x = 0; x < m_NrQuadsX; x++)
        {
            float xClamp = Anwill::Utils::NormalizeBetween0And1(x, 0u, m_NrQuadsY);
            float yClamp = Anwill::Utils::NormalizeBetween0And1(y, 0u, m_NrQuadsX);
            Anwill::Math::Vec3f color(xClamp, yClamp, 0.5f);

            m_SlowColorShader->Bind();
            m_SlowColorShader->SetUniformVec3f(color, "u_Color");
            m_SlowColorShader->Unbind();

            Anwill::Renderer2D::Submit(m_SlowColorShader, m_QuadColorMesh,
                                       transform);
            transform = Anwill::Math::Mat4f::Translate(transform,
                                                       {m_QuadWidth, 0.0f, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {-static_cast<float>(m_CanvasWidth),
                                                    m_QuadHeight, 0.0f});
    }
    m_SlowColorShader->Unbind();

    transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                           {m_QuadWidth * 10,
                                            m_QuadHeight * 10, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform,
                                               {static_cast<float>(m_CanvasWidth / 2),
                                                static_cast<float>(m_CanvasHeight / 2),
                                                0.0f});
    Anwill::Renderer2D::Submit(m_SlowTextShader, m_QuadTextMesh, transform,
                               m_TestTexture);
}

void BatchRendererHelloWorld::MoveCamera()
{
    float speed = 10.0f;
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
        m_Camera.Move(0.0f, speed);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
        m_Camera.Move(-speed, 0.0f);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
        m_Camera.Move(0.0f, -speed);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
        m_Camera.Move(speed, 0.0f);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D1)) {
        m_IsBatchRendering = true;
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D2)) {
        m_IsBatchRendering = false;
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Down)) {
        m_SpriteAnimation.IncrementFrameDelta(0.02);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Up)) {
        m_SpriteAnimation.IncrementFrameDelta(-0.02);
    }
}