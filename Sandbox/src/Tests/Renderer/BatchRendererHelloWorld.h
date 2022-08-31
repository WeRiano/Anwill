#pragma once

#include "Anwill.h"

class BatchRendererHelloWorld : public Anwill::Layer
{
public:
    BatchRendererHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::OrthographicCamera m_Camera;

    bool m_IsBatchRendering;
    unsigned int m_CanvasWidth, m_CanvasHeight;
    float m_QuadWidth, m_QuadHeight;
    unsigned int m_NrQuadsX, m_NrQuadsY;

    std::shared_ptr<Anwill::Shader> m_SlowTextShader, m_SlowColorShader;
    std::unique_ptr<Anwill::Sprite> m_Sprite;
    Anwill::SpriteAnimation m_SpriteAnimation;
    std::shared_ptr<Anwill::Texture> m_TestTexture;
    Anwill::Mesh m_QuadTextMesh, m_QuadColorMesh;

    void BatchRendering();
    void SlowRendering();
    void MoveCamera();
};
