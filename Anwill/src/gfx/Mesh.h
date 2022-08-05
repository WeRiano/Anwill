#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "math/Vec3f.h"

namespace Anwill {

    class Mesh
    {
    public:
        static Mesh CreateRectMesh(float width, float height, bool texture = false);
        static Mesh CreateCircleMesh();
        static Mesh CreateTriangleMesh(float leftWidth, float rightWidth, float height);

        Mesh();
        Mesh(float* vertices, unsigned int verticesSizeBytes, unsigned int* indices,
             unsigned int indicesCount, const Anwill::BufferLayout& bufferLayout);

        std::vector<Math::Vec3f> GetVertices() const;
        const unsigned int GetIndexBufferCount() const;
        void Bind() const;
        void Unbind() const;

    private:
        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<IndexBuffer> m_IB;
        std::vector<Math::Vec3f> m_Vertices;
    };
}