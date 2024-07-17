#include "BatchRendererHelloWorld.h"

BatchRendererHelloWorld::BatchRendererHelloWorld(unsigned int ups,
                                                 const Anwill::WindowSettings& ws)
    : MovingCameraBaseLayer(ups, ws, 5.0f),
      m_RectShader(Anwill::Shader::Create("Sandbox/assets/shaders/RectColor.glsl")),
      m_CircleShader(Anwill::Shader::Create("Sandbox/assets/shaders/CircleColor.glsl")),
      m_TextureShader(Anwill::Shader::Create("Sandbox/assets/shaders/RectTexture.glsl")),
      m_PrimitiveSize(20.0f, 20.0f),
      m_SpriteSheet(Anwill::SpriteSheet::Create(
              "Sandbox/assets/textures/test_sprite_sheet.png", 64, 48))
{
    m_NumPrimitives = 100;

    m_Camera.Move(100.0f, 0.0f);
}

void BatchRendererHelloWorld::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    ImGui::Begin("Batch renderer");

    static bool batchRendering = true, textureRendering = false;
    ImGui::Checkbox("Batch rendering", &batchRendering);
    ImGui::Checkbox("Texture rendering", &textureRendering);
    if(!textureRendering) {
        ImGui::Checkbox("Render quads", &m_IsRenderingQuads);
        ImGui::Checkbox("Render circles", &m_IsRenderingCircles);
    }
    if(!textureRendering) {
        ImGui::Text("Currently rendering n^2 = %d primitives", m_NumPrimitives * m_NumPrimitives);
        ImGui::SliderInt("n", (int*) &m_NumPrimitives, 0, 300);
    }

    ImGui::End();

    GetStrategy(batchRendering, textureRendering)();

    MovingCameraBaseLayer::Update(timestamp);
}

std::function<void()> BatchRendererHelloWorld::GetStrategy(bool batchRendering, bool textureRendering)
{
    if(batchRendering) {
        if (textureRendering) {
            return AW_BIND_THIS_MEMBER_FUNC(BatchRenderingTextureQuads);
        } else {
            return AW_BIND_THIS_MEMBER_FUNC(BatchRendering);
        }
    } else {
        if (textureRendering) {
            return AW_BIND_THIS_MEMBER_FUNC(SlowRenderingTextureQuads);
        } else {
            return AW_BIND_THIS_MEMBER_FUNC(SlowRendering);
        }
    }
}

void BatchRendererHelloWorld::BatchRendering()
{
    AW_PROFILE_FUNC();
    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(), m_PrimitiveSize);
    for(unsigned int y = 0; y < m_NumPrimitives; y++) {
        for(unsigned int x = 0; x < m_NumPrimitives; x++) {
            float xClamp = Anwill::Math::NormalizeToFloat(x, 0u, m_NumPrimitives);
            float yClamp = Anwill::Math::NormalizeToFloat(y, 0u, m_NumPrimitives);
            Anwill::Math::Vec3f color(xClamp, yClamp, 0.5f);
            if( m_IsRenderingQuads && m_IsRenderingCircles) {
                if( (y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1) ) {
                    Anwill::Renderer2D::PushQuadToBatch(transform, color);
                } else {
                    Anwill::Renderer2D::PushCircleToBatch(transform, color);
                }
            } else if(m_IsRenderingQuads) {
                Anwill::Renderer2D::PushQuadToBatch(transform, color);
            } else if(m_IsRenderingCircles) {
                Anwill::Renderer2D::PushCircleToBatch(transform, color);
            }
            transform = Anwill::Math::Mat4f::Translate(transform, {m_PrimitiveSize.X, 0.0f, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {-static_cast<float>(m_NumPrimitives * m_PrimitiveSize.X),
                                                    m_PrimitiveSize.Y, 0.0f});
    }

    Anwill::Renderer2D::DrawBatch();
}

void BatchRendererHelloWorld::SlowRendering()
{
    AW_PROFILE_FUNC();
    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(), m_PrimitiveSize);
    for(unsigned int y = 0; y < m_NumPrimitives; y++) {
        for(unsigned int x = 0; x < m_NumPrimitives; x++) {
            float xClamp = Anwill::Math::NormalizeToFloat(x, 0u, m_NumPrimitives);
            float yClamp = Anwill::Math::NormalizeToFloat(y, 0u, m_NumPrimitives);
            Anwill::Math::Vec3f color(xClamp, yClamp, 0.5f);
            Anwill::Shared<Anwill::Shader> shader;
            if( m_IsRenderingQuads && m_IsRenderingCircles) {
                if( (y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1) ) {
                    shader = m_RectShader;
                } else {
                    shader = m_CircleShader;
                }
            } else if(m_IsRenderingQuads) {
                shader = m_RectShader;
            } else if(m_IsRenderingCircles) {
                shader = m_CircleShader;
            }

            shader->Bind();
            shader->SetUniformVec3f(color, "u_Color");
            shader->Unbind();

            Anwill::Renderer2D::SubmitMesh(shader, Anwill::Mesh::GetUnitRectangle(false), transform);
            transform = Anwill::Math::Mat4f::Translate(transform, {m_PrimitiveSize.X, 0.0f, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {-static_cast<float>(m_NumPrimitives * m_PrimitiveSize.X),
                                                    m_PrimitiveSize.Y, 0.0f});
    }
}

void BatchRendererHelloWorld::BatchRenderingTextureQuads()
{
    AW_PROFILE_FUNC();

    const auto textureSize = m_PrimitiveSize * 2;

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(), textureSize);
    for(int x = 0; x < m_SpriteSheet->GetSpriteXCount(); x++) {
        for(int y = 0; y < m_SpriteSheet->GetSpriteYCount(); y++) {
            Anwill::Renderer2D::PushQuadToBatch(transform, m_SpriteSheet->GetSprite(x, y));
            transform = Anwill::Math::Mat4f::Translate(transform, {0.0f, textureSize.Y, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {textureSize.X,
                                                    -textureSize.Y * m_SpriteSheet->GetSpriteYCount(), 0.0f});
    }
    Anwill::Renderer2D::DrawBatch();
}

void BatchRendererHelloWorld::SlowRenderingTextureQuads()
{
    AW_PROFILE_FUNC();

    const auto textureSize = m_PrimitiveSize * 2;

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(), textureSize);
    for(unsigned int x = 0; x < m_SpriteSheet->GetSpriteYCount(); x++) {
        for(unsigned int y = 0; y < m_SpriteSheet->GetSpriteXCount(); y++) {
            Anwill::Renderer2D::SubmitSprite(m_TextureShader, m_SpriteSheet->GetSprite(x, y), transform);
            transform = Anwill::Math::Mat4f::Translate(transform, {0.0f, textureSize.Y, 0.0f});
        }
        transform = Anwill::Math::Mat4f::Translate(transform,
                                                   {textureSize.X,
                                                    -textureSize.Y * m_SpriteSheet->GetSpriteYCount(), 0.0f});
    }
}
