#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "math/Vec3f.h"

namespace Anwill {

    class Mesh
    {
    public:
        static Mesh CreateRectMesh(float width, float height, bool texture = false);
        static Mesh CreateTriangleMesh(const Math::Vec2f& a, const Math::Vec2f& b, const Math::Vec2f& c);
        static Mesh CreateCheckmarkMesh(float width, float height);

        /**
         * Initialize static meshes.
         */
        static void Init();

        static Mesh GetUnitRectangle(bool withTextureCoords);

        Mesh();
        Mesh(float* vertices, unsigned int verticesSizeBytes, unsigned int* indices,
             unsigned int indicesCount, const Anwill::BufferLayout& bufferLayout);

        std::vector<Math::Vec3f> GetVertices() const;
        const unsigned int GetIndexBufferCount() const;
        void Bind() const;
        void Unbind() const;

    private:
        static Mesh s_UnitRectangle, s_UnitTextureRectangle;

        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<IndexBuffer> m_IB;
        std::vector<Math::Vec3f> m_Vertices;
    };
}