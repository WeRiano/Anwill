#include "SpriteAnimationTest.h"

SpriteAnimationTest::SpriteAnimationTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : MovingCameraBaseLayer(ups, ws, 1.0f),
      m_SpriteAnimation(Anwill::Timestamp(1 * 1000000)),
      m_SpriteSheet(Anwill::SpriteSheet::Create("Sandbox/assets/textures/test_sprite_sheet.png", 64, 48)),
      m_AnimationPos(ws.width * 0.5f, ws.height * 0.5f)
{
    m_SpriteAnimation.AddFrames(m_SpriteSheet, 43, 48, 1, 0, 10);
}

void SpriteAnimationTest::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    ImGui::Begin("Sprite animation");

    static float animationDelta = 250.0f;
    ImGui::SliderFloat("Animation delta (MS)", &animationDelta, 10.0f, 500.0f);

    ImGui::End();

    m_SpriteAnimation.SetFrameDelta(Anwill::Timestamp(animationDelta * 1000.0f));
    m_SpriteAnimation.Tick(GetUpdateDelta(timestamp));

    auto transform = Anwill::Math::Mat4f::Scale({}, {600.0f, 600.0f, 0.0f});
    transform = Anwill::Math::Mat4f::Translate(transform, m_AnimationPos);
    Anwill::Renderer2D::PushQuadToBatch(transform, m_SpriteAnimation);

    Anwill::Renderer2D::DrawBatch();

    MovingCameraBaseLayer::Update(timestamp);
}
