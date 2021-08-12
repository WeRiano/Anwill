#pragma once

#include <memory>

#include "GraphicsAPI.h"

namespace Anwill {

    class Renderer
    {
    public:
        static void Init();

        static void SetAPI(GraphicsAPI::API api);
        static GraphicsAPI::API GetAPI();
    private:
        static GraphicsAPI::API s_APIName;
        static std::unique_ptr<GraphicsAPI> s_API;
    };
}