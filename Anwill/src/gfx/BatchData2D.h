#pragma once

#include <vector>
#include <map>
#include <queue>

#include "gfx/IndexBuffer.h"
#include "gfx/VertexBuffer.h"
#include "gfx/VertexArray.h"
#include "gfx/Texture.h"
#include "gfx/Shader.h"
#include "math/Mat4f.h"

namespace Anwill {

    struct BatchData2D {
        // "Global" values that is shared between all types of batches
        inline static unsigned int maxTextureSlots;
        static const unsigned int startTextureID = 0;

        // Nr of pushed quads / circles / lines
        unsigned int elementsPushed;
        unsigned int verticesArrIndex;
        float* verticesArr;

        std::shared_ptr<VertexBuffer> VB;
        std::shared_ptr<VertexArray> VA;
        std::shared_ptr<Shader> shader;

    };

    struct QuadVertex {
        Math::Vec2f pos;
        Math::Vec2f texCoords;
        Math::Vec3f color;
    };

    struct QuadBatchData : public BatchData2D {
        static const unsigned int maxQuads = 20000;
        static const unsigned int vertexAttribCount = 8;
        static const unsigned int quadAttribCount = vertexAttribCount * 4;
        static const unsigned int verticesArrMaxSize = maxQuads * quadAttribCount;
        static const unsigned int indicesMaxElementSize = maxQuads * 6;

        // Currently circles and lines are technically quads, so both the circle and
        // line batch borrow this information from the quad batch

        // Clockwise, starting from bottom left
        inline static const std::array<Math::Vec2f, 4> unitPositions {
                Math::Vec2f{-0.5f, -0.5f},
                Math::Vec2f{-0.5f,  0.5f},
                Math::Vec2f{ 0.5f,  0.5f},
                Math::Vec2f{ 0.5f, -0.5f}
        };

        unsigned int* indicesArr;
        std::shared_ptr<IndexBuffer> IB;

        unsigned int textureNewIDCount;
        std::map<std::shared_ptr<Texture>, unsigned int> textureMap;
        std::queue<std::shared_ptr<Texture>> textureQ;

        unsigned int GetOrGenerateID(const std::shared_ptr<Texture>& texture)
        {
            if(textureMap.contains(texture)) {
                return textureMap[texture];
            } else {
                textureNewIDCount++;
                textureMap[texture] = textureNewIDCount - 1;
                textureQ.emplace(texture);
                return textureNewIDCount - 1;
            }
        }

        void VertexToArr(const QuadVertex& vertex, int textureID)
        {
            verticesArr[verticesArrIndex + 0] = vertex.pos.GetX();
            verticesArr[verticesArrIndex + 1] = vertex.pos.GetY();
            verticesArr[verticesArrIndex + 2] = vertex.texCoords.GetX();
            verticesArr[verticesArrIndex + 3] = vertex.texCoords.GetY();
            verticesArr[verticesArrIndex + 4] = vertex.color.GetX();
            verticesArr[verticesArrIndex + 5] = vertex.color.GetY();
            verticesArr[verticesArrIndex + 6] = vertex.color.GetZ();
            verticesArr[verticesArrIndex + 7] = static_cast<float>(textureID);
            verticesArrIndex += vertexAttribCount;
        }
    };

    struct CircleVertex {
        Math::Vec2f pos;
        Math::Vec2f texCoords;
        Math::Vec3f color;
        Math::Vec2f centre;
        Math::Vec2f radius;
    };

    struct CircleBatchData : public BatchData2D {
        static const unsigned int maxCircles = 20000;
        static const unsigned int vertexAttribCount = 11;
        static const unsigned int circleAttribCount = vertexAttribCount * 4;
        static const unsigned int verticesArrMaxSize = maxCircles * circleAttribCount;

        void VertexToArr(const CircleVertex& vertex)
        {
            verticesArr[verticesArrIndex + 0] = vertex.pos.GetX();
            verticesArr[verticesArrIndex + 1] = vertex.pos.GetY();
            verticesArr[verticesArrIndex + 2] = vertex.texCoords.GetX();
            verticesArr[verticesArrIndex + 3] = vertex.texCoords.GetY();
            verticesArr[verticesArrIndex + 4] = vertex.color.GetX();
            verticesArr[verticesArrIndex + 5] = vertex.color.GetY();
            verticesArr[verticesArrIndex + 6] = vertex.color.GetZ();
            verticesArr[verticesArrIndex + 7] = vertex.centre.GetX();
            verticesArr[verticesArrIndex + 8] = vertex.centre.GetY();
            verticesArr[verticesArrIndex + 9] = vertex.radius.GetX();
            verticesArr[verticesArrIndex + 10] = vertex.radius.GetY();
            verticesArrIndex += vertexAttribCount;
        }
    };
}