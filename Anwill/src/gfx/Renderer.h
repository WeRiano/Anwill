#pragma once

#include <memory>

#include "GraphicsAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Mesh.h"
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
        static void SetViewport(float x, float y, float width, float height);
        static void SetClearColor(const Math::Vec3f& color);
        static void ClearBuffers();

        static void BeginScene(const Camera& camera);
        static void Submit(const std::shared_ptr<Shader>& shader,
                           const Mesh& mesh,
                           const Math::Mat4f& transform);
        static void Submit(const std::shared_ptr<Shader>& shader,
                           const std::shared_ptr<VertexArray>& vertexArray,
                           const std::shared_ptr<IndexBuffer>& indexBuffer,
                           const Math::Mat4f& transform);
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