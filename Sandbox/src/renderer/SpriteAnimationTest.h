#pragma once

#include "Anwill.h"

#include "Base/MovingCameraBaseLayer.h"

class SpriteAnimationTest : public MovingCameraBaseLayer
{
public:
    SpriteAnimationTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    std::shared_ptr<Anwill::SpriteSheet> m_SpriteSheet;
    Anwill::SpriteAnimation m_SpriteAnimation;
    const Anwill::Math::Vec2f m_AnimationPos;
};