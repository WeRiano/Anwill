#pragma once

#include "gfx/GraphicsAPI.h"

namespace Anwill {

    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:
        OpenGLGraphicsAPI();

        void SetViewport(float x, float y, float width, float height) const override;
        void SetClearColor(const Math::Vec3f& color) const override;
        void ClearBuffers() const override;

        void Draw(const Mesh& mesh) override;
        void Draw(const std::shared_ptr<VertexArray>& vertexArray,
                  const std::shared_ptr<IndexBuffer>& indexBuffer) override;
    };
}
