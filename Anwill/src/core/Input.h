#pragma once

#include <memory>

#include "core/KeyCodes.h"
#include "core/MouseCodes.h"
#include "core/Window.h"

namespace Anwill {

    class Input {
    public:
        static void Init(void* nativeWindow);

        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsMouseButtonDown(MouseButtonCode mouseButtonCode);
    };
}