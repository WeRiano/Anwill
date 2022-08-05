#include "Mesh.h"

namespace Anwill {

    Mesh Mesh::CreateRectMesh(float width, float height, bool texture)
    {
        unsigned int arrLength = texture ? 5 * 4 : 3 * 4;
        float* vertices = new float[arrLength];

        if(!texture)
        {
            vertices[0] = -width / 2.0f;
            vertices[1] = -height / 2.0f;
            vertices[2] = 0.0f;

            vertices[3] = -width / 2.0f;
            vertices[4] = height / 2.0f;
            vertices[5] = 0.0f;

            vertices[6] = width / 2.0f;
            vertices[7] = height / 2.0f;
            vertices[8] = 0.0f;

            vertices[9] = width / 2.0f;
            vertices[10] = -height / 2.0f;
            vertices[11] = 0.0f;
        } else {
            vertices[0] = -width / 2.0f;
            vertices[1] = -height / 2.0f;
            vertices[2] = 0.0f;
            vertices[3] = 0.0f;
            vertices[4] = 0.0f;

            vertices[5] = -width / 2.0f;
            vertices[6] = height / 2.0f;
            vertices[7] = 0.0f;
            vertices[8] = 0.0f;
            vertices[9] = 1.0f;

            vertices[10] = width / 2.0f;
            vertices[11] = height / 2.0f;
            vertices[12] = 0.0f;
            vertices[13] = 1.0f;
            vertices[14] = 1.0f;

            vertices[15] = width / 2.0f;
            vertices[16] = -height / 2.0f;
            vertices[17] = 0.0f;
            vertices[18] = 1.0f;
            vertices[19] = 0.0f;
        }

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        std::vector<BufferElement> elements = {
                BufferElement(ShaderDataType::Float3)
        };

        if(texture)
        {
            elements.emplace_back(ShaderDataType::Float2);
        }
        auto bufferLayout = Anwill::BufferLayout(elements);

        Mesh result(vertices, arrLength * sizeof(float), indices, 6, bufferLayout);
        delete[] vertices;
        return result;
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

        std::vector<BufferElement> elements = {
                Anwill::BufferElement(Anwill::ShaderDataType::Float3)
        };
        auto bufferLayout = Anwill::BufferLayout(elements);

        return {vertices, sizeof(vertices), indices, 3, bufferLayout};
    }

    Mesh::Mesh()
    {}

    Mesh::Mesh(float* vertices, unsigned int verticesSizeBytes, unsigned int* indices,
               unsigned int indicesCount, const Anwill::BufferLayout& bufferLayout)
    {
        for(unsigned int i = 0; i < verticesSizeBytes / sizeof(float); i += 3)
        {
            m_Vertices.emplace_back(vertices[i], vertices[i+1], vertices[i+2]);
        }

        std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices,
                                                                verticesSizeBytes);

        m_VA = VertexArray::Create();
        m_VA->AddBuffer(*vb, bufferLayout);
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