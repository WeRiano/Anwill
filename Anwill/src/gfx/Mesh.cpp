#include "Mesh.h"

namespace Anwill {

    Mesh Mesh::CreateRectMesh(float width, float height)
    {
        float vertices[] = {
            -width/2.0f, -height/2.0f,
            -width/2.0f, height/2.0f,
            width/2.0f, height/2.0f,
            width/2.0f, -height/2.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        Mesh mesh;
        std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));

        auto elements = {
                Anwill::BufferElement(Anwill::ShaderDataType::Float2)
        };

        auto bufferLayout = Anwill::BufferLayout(elements);

        mesh.m_VA = VertexArray::Create();
        mesh.m_VA->AddBuffer(*vb.get(), bufferLayout);
        mesh.m_IB = IndexBuffer::Create(indices, 6);

        return mesh;
    }

    // This is probably not nec, just use shader :)
    Mesh Mesh::CreateCircleMesh()
    {
        return Mesh();
    }

    Mesh Mesh::CreateTriangleMesh(float baseWidth, float height)
    {
        float vertices[] = {
                -baseWidth, -height,
                 0.0f,       height,
                 baseWidth,  height
        };

        unsigned int indices[] = {
                0, 1, 2
        };

        Mesh mesh;
        std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));

        auto elements = {
                Anwill::BufferElement(Anwill::ShaderDataType::Float2)
        };

        auto bufferLayout = Anwill::BufferLayout(elements);

        mesh.m_VA = VertexArray::Create();
        mesh.m_VA->AddBuffer(*vb.get(), bufferLayout);
        mesh.m_IB = IndexBuffer::Create(indices, 3);

        return mesh;
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