#pragma once

#include <vector>
#include <map>
#include <queue>

#include "gfx/IndexBuffer.h"
#include "gfx/VertexBuffer.h"
#include "gfx/VertexArray.h"
#include "gfx/Texture.h"
#include "math/Mat4f.h"

namespace Anwill {

    struct QuadVertex {
        Math::Vec2f pos;
        Math::Vec2f texCoords;
        Math::Vec3f color;
    };

    struct BatchData2D {
        // Runtime constants (initialized in BatchDataInit() and then never changed)
        static const unsigned int maxQuads = 20000;
        // This has to be initialized during runtime
        inline static unsigned int maxTextureSlots;
        // Number of floats in 1 quad vertex
        static const unsigned int quadVertexElementCount = 8;
        // Number of floats for 1 quad.
        static const unsigned int quadElementCount = quadVertexElementCount * 4;
        static const unsigned int quadVerticesArrMaxSize = maxQuads * quadElementCount;
        static const unsigned int quadIndicesMaxElementSize = maxQuads * 6;
        static const unsigned int startTextureID = 0;
        // Clockwise, starting from bottom left
        inline static const std::array<Math::Vec2f, 4> baselineQuadPositions {
                Math::Vec2f{-0.5f, -0.5f},
                Math::Vec2f{-0.5f,  0.5f},
                Math::Vec2f{ 0.5f,  0.5f},
                Math::Vec2f{ 0.5f, -0.5f}
        };

        unsigned int quadsPushed;
        unsigned int quadVerticesArrIndex;
        float quadVerticesArr[quadVerticesArrMaxSize];

        std::shared_ptr<VertexBuffer> quadVB;

        std::shared_ptr<VertexArray> quadVA;

        unsigned int* quadIndices;
        std::shared_ptr<IndexBuffer> quadIB;

        // Not using this until we establish "native" shaders
        //std::shared_ptr<Shader> quadShader;

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

        void QuadVertexToArr(const QuadVertex& vertex, int textureID)
        {
            quadVerticesArr[quadVerticesArrIndex + 0] = vertex.pos.GetX();
            quadVerticesArr[quadVerticesArrIndex + 1] = vertex.pos.GetY();
            quadVerticesArr[quadVerticesArrIndex + 2] = vertex.texCoords.GetX();
            quadVerticesArr[quadVerticesArrIndex + 3] = vertex.texCoords.GetY();
            quadVerticesArr[quadVerticesArrIndex + 4] = vertex.color.GetX();
            quadVerticesArr[quadVerticesArrIndex + 5] = vertex.color.GetY();
            quadVerticesArr[quadVerticesArrIndex + 6] = vertex.color.GetZ();
            quadVerticesArr[quadVerticesArrIndex + 7] = static_cast<float>(textureID);
            quadVerticesArrIndex += quadVertexElementCount;
        }
    };
}