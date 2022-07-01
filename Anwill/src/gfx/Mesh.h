#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"

namespace Anwill {

    class Mesh
    {
    private:
        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<IndexBuffer> m_IB;

    public:
        static Mesh CreateRectMesh(float width, float height);
        static Mesh CreateCircleMesh();
        static Mesh CreateTriangleMesh(float baseWidth, float height);

        const unsigned int GetIndexBufferCount() const;
        void Bind() const;
        void Unbind() const;

    };
}