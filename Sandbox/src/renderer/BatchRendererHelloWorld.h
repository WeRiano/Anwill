#pragma once

#include "Base/MovingCameraBaseLayer.h"

#include "Anwill.h"

class BatchRendererHelloWorld : public MovingCameraBaseLayer
{
public:
    BatchRendererHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    unsigned int m_CanvasWidth, m_CanvasHeight;
    float m_QuadWidth, m_QuadHeight;
    unsigned int m_NrQuadsX, m_NrQuadsY;

    // Single quad shaders for single quad rendering (which is repeated)
    std::shared_ptr<Anwill::Shader> m_SlowTextShader, m_SlowColorShader;

    std::shared_ptr<Anwill::SpriteSheet> m_SpriteSheet;

    std::shared_ptr<Anwill::Texture> m_TestTexture;

    std::function<void()> m_TestFunc;

    void BatchRendering() const;
    void SlowRendering();

    void BatchRenderingTextureQuads();
    void SlowRenderingTextureQuads();

    void BatchRenderingEllipses();
    void SwapTest();
};
