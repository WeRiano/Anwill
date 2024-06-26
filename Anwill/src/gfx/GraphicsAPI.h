#pragma once

#include <memory>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Font.h"
#include "math/Vec3f.h"

namespace Anwill {

    class GraphicsAPI
    {
    public:
        enum class API
        {
            OpenGL = 1
        };

        static std::shared_ptr<GraphicsAPI> Create(API api);

        virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const = 0;
        virtual void SetScissor(const Math::Vec2f& pos, const Math::Vec2f& size) const = 0;
        virtual void SetDefaultScissor() const = 0;
        virtual void PushScissor(const Math::Vec2f& pos, const Math::Vec2f& size) = 0;
        virtual void EndScissor() = 0;
        virtual void SetClearColor(const Math::Vec3f& color) const = 0;
        virtual void ClearBuffers() const = 0;

        virtual void DrawFont(Font& font, const std::string& text) = 0;
        virtual void DrawMesh(const Mesh& mesh) = 0;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray,
                                 const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
        virtual void DrawLine(const std::shared_ptr<VertexArray>& vertexArray) = 0;
        virtual void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, unsigned int vertexCount) = 0;

        virtual std::shared_ptr<Shader> CreateQuadBatchShader() const = 0;
        virtual std::shared_ptr<Shader> CreateCircleBatchShader() const = 0;
    };
}