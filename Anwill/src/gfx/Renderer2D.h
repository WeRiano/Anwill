#pragma once

#include "gfx/BatchData2D.h"
#include "gfx/Renderer.h" // So that we can use all the "global setting functions" that
                          // are universal, so to speak (same for 2D and 3D)
#include "gfx/SpriteAnimation.h"

namespace Anwill {

    class Renderer2D
    {
    public:
        static void Init(const std::shared_ptr<GraphicsAPI>& api);

        static void DrawBatch();
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const Math::Vec3f& color);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const Sprite& sprite);
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const SpriteAnimation& animation);
        static void PushCircleToBatch(const Math::Mat4f& transform,
                                      const Math::Vec3f& color);

        static void BeginScene(const Camera& camera);

        static void SubmitText(const std::shared_ptr<Shader>& shader, Font& font,
                               const std::string& text, const Math::Mat4f& transform, float maxWidth = 0);
        static void Submit(const std::shared_ptr<Shader>& shader,
                           const std::shared_ptr<VertexArray>& vertexArray,
                           const std::shared_ptr<IndexBuffer>& indexBuffer,
                           const Math::Mat4f& transform);
        static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh,
                               const Math::Mat4f& transform,
                               const std::shared_ptr<Texture>& texture = nullptr);
        static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh,
                               const Math::Mat4f& transform,
                               const std::vector<std::shared_ptr<Texture>>& textures);
        static void SubmitLines(const std::shared_ptr<Shader>& shader,
                                const std::shared_ptr<VertexArray> vertexArray,
                                const Math::Mat4f transform, unsigned int linesCount);

    private:
        struct SceneData2D {
            Math::Mat4f ViewProjMat;
            Math::Vec2f CameraPos;

            SceneData2D()
                    : ViewProjMat(Math::Mat4f::Identity())
            {}
        };

        static QuadBatchData s_QData;
        static CircleBatchData s_CData;

        static SceneData2D s_SceneData;
        static std::shared_ptr<GraphicsAPI> s_API;

        static void BatchDataInit();
        static void DrawQuadBatch();
        static void DrawCircleBatch();
    };
}
