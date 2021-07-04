#pragma once

#include <functional>
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

        virtual void Update() = 0;
        virtual void Terminate() = 0;
    };

}