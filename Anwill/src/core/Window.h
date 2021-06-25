#pragma once

#include <memory>

#include "PlatDef.h"

namespace Anwill {

    struct WindowSettings
    {
        unsigned int width, height;
    };

    class Window
    {
    public:
        static std::unique_ptr<Window> Create(WindowSettings ws);
    };

}