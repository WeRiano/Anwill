#pragma once

#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <array>

#include "GraphicsAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "Font.h"
#include "math/Mat4f.h"
#include "math/Vec3f.h"

namespace Anwill {

    struct QuadVertex {
        Math::Vec2f pos;
        Math::Vec2f texCoords;
        Math::Vec3f color;

        QuadVertex(Math::Vec2f pos, Math::Vec2f texCoords, Math::Vec3f color)
            : pos(pos), texCoords(texCoords), color(color)
        {}
    };

    struct Quad {
        QuadVertex p0, p1, p2, p3; // clockwise, starting bottom left

        Quad(QuadVertex p0, QuadVertex p1, QuadVertex p2, QuadVertex p3)
            : p0(p0), p1(p1), p2(p2), p3(p3)
        {}
    };

    struct BatchData2D {
        // Runtime constants (initialized in BatchDataInit() and then never changed)
        static const unsigned int maxQuads = 20000;
        // This has to be initialized during runtime
        static unsigned int maxTextureSlots;
        // Number of floats in 1 quad vertex
        static const unsigned int quadVertexElementCount = 8;
        // Number of floats for 1 quad.
        static const unsigned int quadElementCount = quadVertexElementCount * 4;
        static const unsigned int quadVerticesArrMaxSize = maxQuads * quadElementCount;
        static const unsigned int quadIndicesMaxElementSize = maxQuads * 6;
        static const unsigned int startTextureID = 1;
        // Clockwise, starting from bottom left
        static const std::array<Math::Vec2f, 4> baselineQuadPositions;

        unsigned int quadsPushed;
        // Holds vertices based on texture id. We can use a vector as outer data storage
        // because we create texture id's incrementally anyway.
        // We don't have to store the texture id since that is the outer vector index we
        // are at
        std::vector<std::vector<Quad>> quadsByTexture;
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
                quadsByTexture.emplace_back();
                return textureNewIDCount - 1;
            }
        }

        void QuadVertexToArr(const QuadVertex& vertex, int textureID,
                             unsigned int offset)
        {
            quadVerticesArr[offset + 0] = vertex.pos.GetX();
            quadVerticesArr[offset + 1] = vertex.pos.GetY();
            quadVerticesArr[offset + 2] = vertex.texCoords.GetX();
            quadVerticesArr[offset + 3] = vertex.texCoords.GetY();
            quadVerticesArr[offset + 4] = vertex.color.GetX();
            quadVerticesArr[offset + 5] = vertex.color.GetY();
            quadVerticesArr[offset + 6] = vertex.color.GetZ();
            quadVerticesArr[offset + 7] = static_cast<float>(textureID);
        }

        void QuadToArr(const Quad& quad, int textureID, unsigned int offset)
        {
            QuadVertexToArr(quad.p0, textureID, offset + quadVertexElementCount * 0);
            QuadVertexToArr(quad.p1, textureID, offset + quadVertexElementCount * 1);
            QuadVertexToArr(quad.p2, textureID, offset + quadVertexElementCount * 2);
            QuadVertexToArr(quad.p3, textureID, offset + quadVertexElementCount * 3);
        }
    };

    class Renderer
    {
    public:
        static void Init();

        static GraphicsAPI::API GetAPI();
        static void SetAPI(GraphicsAPI::API api);

        // Static entrypoints for GraphicAPI responsibilities
        static void SetViewport(unsigned int x, unsigned int y,
                                unsigned int width, unsigned int height);
        static void SetClearColor(const Math::Vec3f& color);
        static void ClearBuffers();

        // Batch rendering stuff

        // Do we need these?
        //static void BeginBatch();
        static void DrawBatch(const std::shared_ptr<Shader>& shader);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const Math::Vec3f& color);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const std::shared_ptr<Texture>& texture,
                                    float texX0 = 0.0f, float texY0 = 0.0f,
                                    float texX1 = 1.0f, float texY1 = 1.0f);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const std::shared_ptr<SpriteSheet>& spriteSheet,
                                    unsigned int spriteXPos, unsigned int spriteYPos);

        static void BeginScene(const Camera& camera);

        static void Submit(const std::shared_ptr<Shader>& shader, Font& font,
                           const std::string& text, const Math::Mat4f& transform);
        static void Submit(const std::shared_ptr<Shader>& shader,
                           const std::shared_ptr<VertexArray>& vertexArray,
                           const std::shared_ptr<IndexBuffer>& indexBuffer,
                           const Math::Mat4f& transform);
        static void Submit(const std::shared_ptr<Shader>& shader, const Mesh& mesh,
                           const Math::Mat4f& transform,
                           const std::shared_ptr<Texture>& texture = nullptr);
        static void Submit(const std::shared_ptr<Shader>& shader, const Mesh& mesh,
                           const Math::Mat4f& transform,
                           const std::vector<std::shared_ptr<Texture>>& textures);

    private:
        struct SceneData {
            Math::Mat4f ViewProjMat;

            SceneData()
                : ViewProjMat(Math::Mat4f::Identity())
            {}
        };

        static GraphicsAPI::API s_APIName;
        static std::unique_ptr<GraphicsAPI> s_API;
        static SceneData s_SceneData;
        static BatchData2D s_B2Data;

        static void BatchDataInit();
    };
}