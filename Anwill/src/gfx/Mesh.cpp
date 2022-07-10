#include "Mesh.h"

namespace Anwill {

    Mesh Mesh::CreateRectMesh(float width, float height)
    {
        float vertices[] = {
            -width / 2.0f, -height / 2.0f, 0.0f,
            -width / 2.0f, height / 2.0f, 0.0f,
            width / 2.0f, height / 2.0f, 0.0f,
            width / 2.0f, -height / 2.0f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        auto elements = {
                Anwill::BufferElement(Anwill::ShaderDataType::Float3)
        };
        auto bufferLayout = Anwill::BufferLayout(elements);

        return {vertices, sizeof(vertices), indices, 6, bufferLayout};
    }

    // This is probably not nec, just use shader :)
    Mesh Mesh::CreateCircleMesh()
    {
        return {};
    }

    Mesh Mesh::CreateTriangleMesh(float baseWidth, float rightWidth, float height)
    {
        float vertices[] = {
                -baseWidth, -height / 2.0f, 0.0f,
                 0.0f,       height / 2.0f, 0.0f,
                 rightWidth,  height / 2.0f, 0.0f
        };

        unsigned int indices[] = {
                0, 1, 2
        };

        auto elements= {
                Anwill::BufferElement(Anwill::ShaderDataType::Float3)
        };
        auto bufferLayout = Anwill::BufferLayout(elements);

        return {vertices, sizeof(vertices), indices, 3, bufferLayout};
    }

    Mesh::Mesh()
    {}

    Mesh::Mesh(float* vertices, unsigned int verticesSizeBytes, unsigned int* indices, unsigned int indicesCount,
               const Anwill::BufferLayout& bufferLayout)
    {
        for(unsigned int i = 0; i < verticesSizeBytes / sizeof(float); i += 3)
        {
            m_Vertices.emplace_back(vertices[i], vertices[i+1], vertices[i+2]);
        }

        std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, verticesSizeBytes);

        m_VA = VertexArray::Create();
        m_VA->AddBuffer(*vb.get(), bufferLayout);
        m_IB = IndexBuffer::Create(indices, indicesCount);
    }

    std::vector<Math::Vec3f> Mesh::GetVertices() const
    {
        return m_Vertices;
    }

    const unsigned int Mesh::GetIndexBufferCount() const
    {
        return m_IB->GetCount();
    }

    void Mesh::Bind() const
    {
        m_VA->Bind();
        m_IB->Bind();
    }

    void Mesh::Unbind() const
    {
        m_VA->Unbind();
        m_IB->Unbind();
    }
}