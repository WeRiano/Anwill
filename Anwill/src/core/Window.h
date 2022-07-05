#pragma once

#include <functional>
#include <memory>

#include "PlatDef.h"

namespace Anwill {

    struct WindowSettings
    {
        unsigned int width = 1280;
        unsigned int height = 720;
        const char* title = "Anwill Engine";
    };

    class Window
    {
    public:
        static std::unique_ptr<Window> Create(const WindowSettings& ws);

        virtual void Update() = 0;
        virtual void Terminate() = 0;
        virtual void* GetNativeWindow() const = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
    };

}