#pragma once

#include "base/MovingCameraBaseLayer.h"
#include "Anwill.h"

class TextureTest : public MovingCameraBaseLayer
{
public:
    TextureTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Mesh m_Mesh;
    Anwill::Shared<Anwill::Texture> m_Texture;
    Anwill::Shared<Anwill::Shader> m_Shader;
    Anwill::Math::Vec2f m_TextureSize;
    const Anwill::Math::Vec2f m_TexturePos;
};
