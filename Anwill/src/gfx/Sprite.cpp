#include "gfx/Mesh.h"
#include "gfx/Sprite.h"

namespace Anwill {

    Sprite::Sprite(std::shared_ptr<Texture> texture, const QuadTexCoords texCoords)
        : texture(std::move(texture)), texCoords(texCoords)
    {}

    Mesh Sprite::GetQuadMesh() const
    {
        unsigned int arrLength = 5 * 4;
        float* vertices = new float[arrLength];

        vertices[0] = -0.5f;
        vertices[1] = -0.5f;
        vertices[2] = 0.0f;
        vertices[3] = texCoords.x0;
        vertices[4] = texCoords.y0;

        vertices[5] = -0.5f;
        vertices[6] = 0.5f;
        vertices[7] = 0.0f;
        vertices[8] = texCoords.x0;
        vertices[9] = texCoords.y1;

        vertices[10] = 0.5f;
        vertices[11] = 0.5f;
        vertices[12] = 0.0f;
        vertices[13] = texCoords.x1;
        vertices[14] = texCoords.y1;

        vertices[15] = 0.5f;
        vertices[16] = -0.5f;
        vertices[17] = 0.0f;
        vertices[18] = texCoords.x1;
        vertices[19] = texCoords.y0;

        unsigned int indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        std::vector<BufferElement> elements = {
                BufferElement(ShaderDataType::Float3)
        };

        if (texture)
        {
            elements.emplace_back(ShaderDataType::Float2);
        }
        auto bufferLayout = Anwill::BufferLayout(elements);

        Mesh result(vertices, arrLength * sizeof(float), indices, 6, bufferLayout);
        delete[] vertices;
        return result;
    }
}