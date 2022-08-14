#pragma once

#include "gfx/BatchData2D.h"
#include "gfx/Renderer.h" // So that we can use all the "global setting functions" that
                          // are universal, so to speak (same for 2D and 3D)

namespace Anwill {

    class Renderer2D
    {
    public:
        static void Init(const std::shared_ptr<GraphicsAPI>& api);

        static void DrawBatch(const std::shared_ptr<Shader>& shader);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const std::shared_ptr<Shader>& shader,
                                    const Math::Vec3f& color);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const std::shared_ptr<Shader>& shader,
                                    const std::shared_ptr<Texture>& texture,
                                    float texX0 = 0.0f, float texY0 = 0.0f,
                                    float texX1 = 1.0f, float texY1 = 1.0f);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const std::shared_ptr<Shader>& shader,
                                    const std::shared_ptr<SpriteSheet>& spriteSheet,
                                    unsigned int spriteXPos, unsigned int spriteYPos,
                                    int pixelLeftPad = 0, int pixelRightPad = 0,
                                    int pixelBottomPad = 0, int pixelTopPad = 0);

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
        struct SceneData2D {
            Math::Mat4f ViewProjMat;

            SceneData2D()
                    : ViewProjMat(Math::Mat4f::Identity())
            {}
        };

        static BatchData2D s_BData;
        static SceneData2D s_SceneData;
        static std::shared_ptr<GraphicsAPI> s_API;

        static void BatchDataInit();
    };
}
