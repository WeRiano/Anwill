#pragma once

#include "Anwill.h"

class BatchRendererHelloWorld : public Anwill::Layer
{
public:
    BatchRendererHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::OrthographicCamera m_Camera;

    std::shared_ptr<Anwill::Shader> m_QuadShader;
    std::shared_ptr<Anwill::SpriteSheet> m_TestSpriteSheet;
    std::shared_ptr<Anwill::Texture> m_TestTexture;

    void MoveCamera();
};
