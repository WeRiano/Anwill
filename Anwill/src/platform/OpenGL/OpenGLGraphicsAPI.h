#pragma once

#include <stack>

#include "gfx/GraphicsAPI.h"
#include "gfx/Font.h"

namespace Anwill {

    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:
        OpenGLGraphicsAPI();

        void SetViewport(unsigned int x, unsigned int y, unsigned int width,
                         unsigned int height) const override;
        void SetScissor(const Math::Vec2f& pos, const Math::Vec2f& size) const override;
        void SetDefaultScissor() const override;
        void PushScissor(const Math::Vec2f& pos, const Math::Vec2f& size) override;
        void EndScissor() override;
        void SetClearColor(const Math::Vec3f& color) const override;
        void ClearBuffers() const override;

        void DrawFont(Font& font, const std::string& text) override;
        void DrawMesh(const Mesh& mesh) override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray,
                         const std::shared_ptr<IndexBuffer>& indexBuffer) override;
        void DrawLine(const std::shared_ptr<VertexArray>& vertexArray) override;
        void DrawLines(const std::shared_ptr<VertexArray>& vertexArray,
                       unsigned int vertexCount) override;

        std::shared_ptr<Shader> CreateQuadBatchShader() const override;
        std::shared_ptr<Shader> CreateCircleBatchShader() const override;

    private:
        struct Scissor {
        public:
            Math::Vec2f pos;
            Math::Vec2f size;

            void Set() const;
        };

        std::stack<Scissor> m_ScissorStack;
    };
}
