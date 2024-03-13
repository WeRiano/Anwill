#pragma once

#include "gfx/BatchData2D.h"
// So that we can use all the "global setting functions" that
// are universal, so to speak (same for 2D and 3D)
#include "gfx/Renderer.h"
#include "gfx/SpriteAnimation.h"

namespace Anwill {

    class Renderer2D
    {
    public:
        static void Init(const std::shared_ptr<GraphicsAPI>& api);

        static void BeginScene(const Camera& camera);

        /**
         * Add a (texture-less) color-quad to the batch.
         * @param transform Quad transform.
         * @param color Quad color.
         */
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const Math::Vec3f& color);
        /**
         * Add a sprite (in the form of a quad) to the batch.
         * @param transform Quad transform.
         * @param sprite Quad sprite.
         */
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const Sprite& sprite);
        /**
         * Add a 2D sprite animation (in the form of a quad) to the batch.
         * @param transform Quad transform.
         * @param animation Quad animation.
         */
        static void PushQuadToBatch(const Math::Mat4f& transform,
                                    const SpriteAnimation& animation);
        /**
         * Add a (texture-less) color-circle to the batch.
         * @param transform Circle transform.
         * @param color Circle color.
         */
        static void PushCircleToBatch(const Math::Mat4f& transform,
                                      const Math::Vec3f& color);
        /**
         * Draw the batch. This is split into 2 different draw calls: circles and quads.
         * Additional draw calls may also occur depending on the number of textures used
         * and the number of texture slots supported.
         */
        static void DrawBatch();

        static void SubmitText(const std::shared_ptr<Shader>& shader, Font& font,
                               const std::string& text, const Math::Mat4f& transform, float maxWidth = 0);
        static void SubmitVertices(const std::shared_ptr<Shader>& shader,
                                   const std::shared_ptr<VertexArray>& vertexArray,
                                   const std::shared_ptr<IndexBuffer>& indexBuffer,
                                   const Math::Mat4f& transform);
        static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh,
                               const Math::Mat4f& transform,
                               const std::shared_ptr<Texture>& texture = nullptr);
        static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh,
                               const Math::Mat4f& transform,
                               const std::vector<std::shared_ptr<Texture>>& textures);
        static void SubmitSprite(const std::shared_ptr<Shader>& shader,
                                 const Sprite& sprite,
                                 const Math::Mat4f& transform);
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
