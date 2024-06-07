#include "BatchRendererHelloWorld.h"

BatchRendererHelloWorld::BatchRendererHelloWorld(unsigned int ups,
                                                 const Anwill::WindowSettings& ws)
    : MovingCameraBaseLayer(ups, ws, 5.0f),
      m_SpriteSheet(Anwill::SpriteSheet::Create(
              "Sandbox/assets/textures/test_sprite_sheet.png", 64, 48)),
      m_TestFunc(AW_BIND_THIS_MEMBER_FUNC(BatchRendering))
{
    m_SlowTextShader = Anwill::Shader::Create("Sandbox/assets/shaders/RectTexture.glsl");
    m_SlowColorShader = Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl");

    m_QuadWidth = 20.0f;
    m_QuadHeight = 20.0f;
    m_CanvasWidth = ws.width * 2;
    m_CanvasHeight = ws.height * 2;
    m_NrQuadsX = static_cast<unsigned int>(m_CanvasWidth) /
            static_cast<unsigned int>(m_QuadWidth);
    m_NrQuadsY = static_cast<unsigned int>(m_CanvasHeight) /
            static_cast<unsigned int>(m_QuadHeight);

    m_Camera.Move(100.0f, 0.0f);
}

void BatchRendererHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    m_TestFunc();

    SwapTest();

    MovingCameraBaseLayer::Update(timestamp);
}

void BatchRendererHelloWorld::BatchRendering() const
{
    AW_PROFILE_FUNC();
    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {m_QuadWidth, m_QuadHeight, 0.0f});
    for(unsigned int y = 0; y < m_NrQuadsY; y++)
    {
        for(unsigned int x = 0; x < m_NrQuadsX; x++)
        {
            float xClamp = Anwill::Math::NormalizeToFloat(x, 0u, m_NrQuadsY);
            float yClamp = Anwill::Math::NormalizeToFloat(y, 0u, m_NrQuadsX);
            Anwill::Math::Vec3f color(xClamp, yClamp, 0.5f);
            if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1))
            {
                Anwill::Renderer2D::PushQuadToBatch(transform, color);
            } else {
                Anwill::Renderer2D::PushCircleToBatch(transform, color);
            }
            transform = Anwill::Math::Mat4f::Translate(transform,
                                                       {m_QuadWidth, 0.0f, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {-static_cast<float>(m_CanvasWidth), m_QuadHeight, 0.0f});
    }

    Anwill::Renderer2D::DrawBatch();
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
            float xClamp = Anwill::Math::NormalizeToFloat(x, 0u, m_NrQuadsY);
            float yClamp = Anwill::Math::NormalizeToFloat(y, 0u, m_NrQuadsX);
            Anwill::Math::Vec3f color(xClamp, yClamp, 0.5f);

            m_SlowColorShader->Bind();
            m_SlowColorShader->SetUniformVec3f(color, "u_Color");
            m_SlowColorShader->Unbind();

            Anwill::Renderer2D::SubmitMesh(m_SlowColorShader, Anwill::Mesh::GetUnitRectangle(false),
                                           transform);
            transform = Anwill::Math::Mat4f::Translate(transform,
                                                       {m_QuadWidth, 0.0f, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {-static_cast<float>(m_CanvasWidth),
                                                    m_QuadHeight, 0.0f});
    }
}

void BatchRendererHelloWorld::BatchRenderingTextureQuads() {
    AW_PROFILE_FUNC();

    const float textureWidth = m_QuadWidth * 2;
    const float textureHeight = m_QuadHeight * 2;

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {textureWidth, textureHeight, 0.0f});
    for(int x = 0; x < m_SpriteSheet->GetSpriteXCount(); x++) {
        for(int y = 0; y < m_SpriteSheet->GetSpriteYCount(); y++) {
            Anwill::Renderer2D::PushQuadToBatch(transform, m_SpriteSheet->GetSprite(x, y));
            transform = Anwill::Math::Mat4f::Translate(transform, {0.0f, textureHeight, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {textureWidth,
                                                    -textureHeight * m_SpriteSheet->GetSpriteYCount(),
                                                    0.0f});
    }

    Anwill::Renderer2D::DrawBatch();
}

void BatchRendererHelloWorld::SlowRenderingTextureQuads()
{
    AW_PROFILE_FUNC();

    const float textureWidth = m_QuadWidth * 2;
    const float textureHeight = m_QuadHeight * 2;

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                                {textureWidth, textureHeight, 0.0f});
    for(unsigned int x = 0; x < m_SpriteSheet->GetSpriteYCount(); x++)
    {
        for(unsigned int y = 0; y < m_SpriteSheet->GetSpriteXCount(); y++)
        {
            float xClamp = Anwill::Math::NormalizeToFloat(x, 0u, m_SpriteSheet->GetSpriteXCount());
            float yClamp = Anwill::Math::NormalizeToFloat(y, 0u, m_SpriteSheet->GetSpriteYCount());
            Anwill::Math::Vec3f color(xClamp, yClamp, 0.5f);

            m_SlowColorShader->Bind();
            m_SlowColorShader->SetUniformVec3f(color, "u_Color");
            m_SlowColorShader->Unbind();

            Anwill::Renderer2D::SubmitSprite(m_SlowTextShader, m_SpriteSheet->GetSprite(x, y),
                                           transform);
            transform = Anwill::Math::Mat4f::Translate(transform, {0.0f, textureHeight, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {textureWidth,
                                                    -textureHeight * m_SpriteSheet->GetSpriteYCount(),
                                                    0.0f});
    }
}

void BatchRendererHelloWorld::BatchRenderingEllipses()
{
    // TODO:
    /*
    transform = Anwill::Math::Mat4f::Scale(transform, {2.0f, 1.0f, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform, {400.0f, 0.0f, 0.0f});
    Anwill::Renderer2D::PushCircleToBatch(transform, {0.3f, 0.4f, 0.9f});
    Anwill::Renderer2D::DrawBatch();
     */
}

void BatchRendererHelloWorld::SwapTest()
{
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D1)) {
        m_TestFunc = AW_BIND_THIS_MEMBER_FUNC(BatchRendering);
    } else if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D2)) {
        m_TestFunc = AW_BIND_THIS_MEMBER_FUNC(SlowRendering);
    } else if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D3)) {
        m_TestFunc = AW_BIND_THIS_MEMBER_FUNC(BatchRenderingTextureQuads);
    } else if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D4)) {
        m_TestFunc = AW_BIND_THIS_MEMBER_FUNC(SlowRenderingTextureQuads);
    } else if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D5)) {
        m_TestFunc = AW_BIND_THIS_MEMBER_FUNC(BatchRenderingEllipses);
    }
}
