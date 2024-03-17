#include "SpriteAnimationTest.h"

SpriteAnimationTest::SpriteAnimationTest(unsigned int ups, const Anwill::WindowSettings& ws)
    : MovingCameraBaseLayer(ups, ws, 1.0f),
      m_SpriteAnimation(Anwill::Timestamp(1  * 1000000)),
      m_SpriteSheet(Anwill::SpriteSheet::Create("Sandbox/assets/textures/test_sprite_sheet.png", 64, 48))
{
    m_SpriteAnimation.AddFrames(m_SpriteSheet, 43, 48, 1, 0, 10);
}

void SpriteAnimationTest::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    ChangeAnimationSpeed();

    m_SpriteAnimation.Tick(GetUpdateDelta(timestamp));

    auto transform = Anwill::Math::Mat4f::Scale(Anwill::Math::Mat4f::Identity(),
                                           {200.0f, 200.0f, 0.0f});
    Anwill::Renderer2D::PushQuadToBatch(transform, m_SpriteAnimation);

    Anwill::Renderer2D::DrawBatch();

    MovingCameraBaseLayer::Update(timestamp);
}

void SpriteAnimationTest::ChangeAnimationSpeed()
{
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Down)) {
        m_SpriteAnimation.ChangeFrameDelta(0.02);
    }
    if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::Up)) {
        m_SpriteAnimation.ChangeFrameDelta(-0.02);
    }
}
