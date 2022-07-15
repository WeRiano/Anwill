#pragma once

#include "gfx/GraphicsAPI.h"

namespace Anwill {

    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:
        OpenGLGraphicsAPI();

        void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const override;
        void SetClearColor(const Math::Vec3f& color) const override;
        void ClearBuffers() const override;

        void Draw(const Mesh& mesh) override;
        void Draw(const std::shared_ptr<VertexArray>& vertexArray,
                  const std::shared_ptr<IndexBuffer>& indexBuffer) override;
    };
}
