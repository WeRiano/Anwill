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

    struct BatchRenderStats {
        unsigned int drawCalls = 0;
        unsigned int drawnCircles = 0;
        unsigned int drawnQuads = 0;
    };

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
        float textureID;
    };

    struct QuadBatchData : public BatchData2D {
        static const unsigned int maxQuads = 30000;
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

        void VertexToArr(const QuadVertex& vertex)
        {
            verticesArr[verticesArrIndex + 0] = vertex.pos.X;
            verticesArr[verticesArrIndex + 1] = vertex.pos.Y;
            verticesArr[verticesArrIndex + 2] = vertex.texCoords.X;
            verticesArr[verticesArrIndex + 3] = vertex.texCoords.Y;
            verticesArr[verticesArrIndex + 4] = vertex.color.X;
            verticesArr[verticesArrIndex + 5] = vertex.color.Y;
            verticesArr[verticesArrIndex + 6] = vertex.color.Z;
            verticesArr[verticesArrIndex + 7] = vertex.textureID;
            verticesArrIndex += vertexAttribCount;
        }
    };

    struct CircleVertex {
        Math::Vec2f worldPosition;
        Math::Vec2f localPosition;
        Math::Vec3f color;
    };

    struct CircleBatchData : public BatchData2D {
        static const unsigned int maxCircles = 30000;
        static const unsigned int vertexAttribCount = 7;
        static const unsigned int circleAttribCount = vertexAttribCount * 4;
        static const unsigned int verticesArrMaxSize = maxCircles * circleAttribCount;

        void VertexToArr(const CircleVertex& vertex)
        {
            verticesArr[verticesArrIndex + 0] = vertex.worldPosition.X;
            verticesArr[verticesArrIndex + 1] = vertex.worldPosition.Y;
            verticesArr[verticesArrIndex + 2] = vertex.localPosition.X;
            verticesArr[verticesArrIndex + 3] = vertex.localPosition.Y;
            verticesArr[verticesArrIndex + 4] = vertex.color.X;
            verticesArr[verticesArrIndex + 5] = vertex.color.Y;
            verticesArr[verticesArrIndex + 6] = vertex.color.Z;
            verticesArrIndex += vertexAttribCount;
        }
    };
}