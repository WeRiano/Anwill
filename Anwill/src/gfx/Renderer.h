#pragma once

#include <memory>

#include "GraphicsAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Font.h"
#include "math/Mat4f.h"
#include "math/Vec3f.h"

namespace Anwill {

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
    };
}