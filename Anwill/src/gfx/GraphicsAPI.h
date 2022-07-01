#pragma once

#include <memory>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "math/Vec3f.h"

namespace Anwill {

    class GraphicsAPI
    {
    public:
        enum class API
        {
            OpenGL = 1
        };

        static std::unique_ptr<GraphicsAPI> Create(API api);

        virtual void SetClearColor(const Math::Vec3f& color) const = 0;
        virtual void ClearBuffers() const = 0;
        virtual void Draw(const Mesh& mesh) = 0;
        virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray,
                          const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
    };
}