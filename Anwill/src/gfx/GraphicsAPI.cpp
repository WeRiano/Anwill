#include "gfx/GraphicsAPI.h"
#include "core/Assert.h"
#include "platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Anwill {

    std::shared_ptr<GraphicsAPI> GraphicsAPI::Create(API api)
    {
        switch(api)
        {
            case API::OpenGL:
                return std::make_shared<OpenGLGraphicsAPI>();
            default:
                AW_ASSERT(false, "No graphics API not supported");
                return nullptr;
        }
    }

}
