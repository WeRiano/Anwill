#include "Mesh.h"

namespace Anwill {

    Mesh Mesh::s_UnitRectangle, Mesh::s_UnitTextureRectangle;

    Mesh Mesh::CreateRectMesh(float width, float height, bool texture)
    {
        unsigned int arrLength = texture ? 4 * 4 : 2 * 4;
        float* vertices = new float[arrLength];

        if(!texture)
        {
            vertices[0] = -width / 2.0f;
            vertices[1] = -height / 2.0f;

            vertices[2] = -width / 2.0f;
            vertices[3] = height / 2.0f;

            vertices[4] = width / 2.0f;
            vertices[5] = height / 2.0f;

            vertices[6] = width / 2.0f;
            vertices[7] = -height / 2.0f;
        } else {
            vertices[0] = -width / 2.0f;
            vertices[1] = -height / 2.0f;
            vertices[2] = 0.0f;
            vertices[3] = 0.0f;

            vertices[4] = -width / 2.0f;
            vertices[5] = height / 2.0f;
            vertices[6] = 0.0f;
            vertices[7] = 1.0f;

            vertices[8] = width / 2.0f;
            vertices[9] = height / 2.0f;
            vertices[10] = 1.0f;
            vertices[11] = 1.0f;

            vertices[12] = width / 2.0f;
            vertices[13] = -height / 2.0f;
            vertices[14] = 1.0f;
            vertices[15] = 0.0f;
        }

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        std::vector<BufferElement> elements = {
                BufferElement(ShaderDataType::Float2)
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

    Mesh Mesh::CreateTriangleMesh(const Math::Vec2f& a, const Math::Vec2f& b, const Math::Vec2f& c)
    {
        float vertices[] = {
                a.X, a.Y, 0.0f,
                b.X, b.Y, 0.0f,
                c.X, c.Y, 0.0f
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

    Mesh Mesh::CreateCheckmarkMesh(float width, float height) {
        // Width is distance between the leftmost vertex (e) and the rightmost vertex (a)
        // Height is distance between the most north vertex (b) and the most south vertex (c)
        Math::Vec2f a = {0.5f * width, 0.375f * height};
        Math::Vec2f b = {a.Y, a.X};
        Math::Vec2f c = {-0.1f * width, -0.5f * height};
        Math::Vec2f d = {b.X - a.X + c.X, b.Y - a.Y + c.Y};
        Math::Vec2f e = {c.Y, c.X};
        Math::Vec2f f = {e.X + a.X - b.X, e.Y + b.Y - a.Y};
        Math::Vec2f g = {f.X - e.X + d.X, f.Y - e.Y + d.Y};

        float vertices[] = {
                a.X, a.Y, 0.0f,
                b.X, b.Y, 0.0f,
                c.X, c.Y, 0.0f,
                d.X, d.Y, 0.0f,
                e.X, e.Y, 0.0f,
                f.X, f.Y, 0.0f,
                g.X, g.Y, 0.0f
        };

        unsigned int indices[] = {
                0, 1, 2, // a, b, c
                3, 2, 1, // d, c, b

                6, 5, 3, // g, f, d
                4, 3, 5 // e, d, f
        };

        std::vector<BufferElement> elements = {
                Anwill::BufferElement(Anwill::ShaderDataType::Float3)
        };
        auto bufferLayout = Anwill::BufferLayout(elements);

        return {vertices, sizeof(vertices), indices, 12, bufferLayout};
    }

    void Mesh::Init()
    {
        s_UnitRectangle = CreateRectMesh(1.0f, 1.0f, false);
        s_UnitTextureRectangle = CreateRectMesh(1.0f, 1.0f, true);
    }

    Mesh Mesh::GetUnitRectangle(bool withTextureCoords)
    {
        return withTextureCoords ? s_UnitTextureRectangle : s_UnitRectangle;
    }

    Mesh::Mesh()
    {}

    Mesh::Mesh(float* vertices, unsigned int verticesSizeBytes, unsigned int* indices,
               unsigned int indicesCount, const Anwill::BufferLayout& bufferLayout)
    {
        // TODO: Assume that first attribute is the vertices. I think this is fair.
        auto vertexCount = bufferLayout.GetElements()[0].count;
        for(unsigned int i = 0; i < verticesSizeBytes / sizeof(float); i += vertexCount)
        {
            m_Vertices.emplace_back(vertices[i], vertices[i+1], vertexCount == 2 ? 0 : vertices[i+3]);
        }

        std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices,verticesSizeBytes);

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