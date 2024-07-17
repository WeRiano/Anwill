#pragma once

#include <functional>
#include "Base/MovingCameraBaseLayer.h"

#include "Anwill.h"

class BatchRendererHelloWorld : public MovingCameraBaseLayer
{
public:
    BatchRendererHelloWorld(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    Anwill::Shared<Anwill::Shader> m_RectShader, m_CircleShader, m_TextureShader;
    Anwill::Math::Vec2f m_PrimitiveSize;
    //float m_QuadWidth, m_QuadHeight;
    unsigned int m_NumPrimitives;
    bool m_IsRenderingQuads, m_IsRenderingCircles;
    std::shared_ptr<Anwill::SpriteSheet> m_SpriteSheet;
    std::shared_ptr<Anwill::Texture> m_TestTexture;

    std::function<void()> GetStrategy(bool batchRendering, bool textureRendering);
    void BatchRendering();
    void SlowRendering();
    void BatchRenderingTextureQuads();
    void SlowRenderingTextureQuads();
};
