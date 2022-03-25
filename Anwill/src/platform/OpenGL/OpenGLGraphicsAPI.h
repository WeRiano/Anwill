#pragma once

#include "gfx/GraphicsAPI.h"

namespace Anwill {

    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:
        OpenGLGraphicsAPI();

        void ClearBuffers();
    };
}
