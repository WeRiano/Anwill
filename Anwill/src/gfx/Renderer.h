#pragma once

#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <array>

#include "gfx/GraphicsAPI.h"
#include "gfx/Shader.h"
#include "gfx/VertexArray.h"
#include "gfx/IndexBuffer.h"
#include "gfx/Camera.h"
#include "gfx/Mesh.h"
#include "gfx/Texture.h"
#include "gfx/SpriteSheet.h"
#include "gfx/Font.h"
#include "math/Mat4f.h"
#include "math/Vec3f.h"

namespace Anwill {

    class Renderer
    {
    public:
        static void Init();

        static GraphicsAPI::API GetAPI();
        static void SetAPI(GraphicsAPI::API api);

        static void SetViewport(unsigned int x, unsigned int y,
                                unsigned int width, unsigned int height);
        static void SetScissor(const Math::Vec2f& pos, const Math::Vec2f& size);
        static void ResetScissor();
        static void SetClearColor(const Math::Vec3f& color);
        static void ClearBuffers();

    private:
        static GraphicsAPI::API s_APIName;
        static std::shared_ptr<GraphicsAPI> s_API;
    };
}