#pragma once

#include <memory>
#include <unordered_map>

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

    struct BatchData2D {
        // Runtime constants (initialized in BatchDataInit() and then never changed)
        unsigned int maxQuads;
        unsigned int maxTextureSlots;
        unsigned int quadVertexCount; // Size of 1 vertex in number of floats
        unsigned int quadVerticesMaxSize; // Size of entire array

        unsigned int quadsPushed;

        float* quadVertices;
        unsigned int quadVerticesIndex;
        std::shared_ptr<VertexBuffer> quadVB;

        std::shared_ptr<VertexArray> quadVA;

        unsigned int* quadIndices; // Don't to store this
        std::shared_ptr<IndexBuffer> quadIB;

        // Not using this until we establish "native" shaders
        //std::shared_ptr<Shader> quadShader;

        unsigned int textureNewIDCount;
        std::unordered_map<std::shared_ptr<Texture>, unsigned int> textureMap;

        unsigned int GetOrGenerateID(const std::shared_ptr<Texture>& texture) {
            if(textureMap.contains(texture)) {
                return textureMap[texture];
            } else {
                textureNewIDCount++;
                textureMap[texture] = textureNewIDCount - 1;
                return textureNewIDCount - 1;
            }
        }
    };

    class Renderer
    {
    public:
        static void Init();

        static GraphicsAPI::API GetAPI();
        static void SetAPI(GraphicsAPI::API api);

        // Static entrypoints for GraphicAPI responsibilities
        static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
        static void SetClearColor(const Math::Vec3f& color);
        static void ClearBuffers();

        // Batch rendering stuff

        // Do we need these?
        //static void BeginBatch();
        static void DrawBatch(const std::shared_ptr<Shader>& shader);
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