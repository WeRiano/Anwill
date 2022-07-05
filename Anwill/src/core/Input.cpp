#include "Input.h"
#include "PlatDef.h"

#ifdef AW_PLATFORM_WINDOWS
#include "platform/Windows/WinInput.h"
#endif

namespace Anwill {

    void Input::Init(void* nativeWindow)
    {
        #ifdef AW_PLATFORM_WINDOWS
            WinInput::Init(static_cast<GLFWwindow*>(nativeWindow));
        #endif
    }

    bool Input::IsKeyPressed(const KeyCode& keyCode)
    {
        #ifdef AW_PLATFORM_WINDOWS
            return WinInput::IsKeyPressed(keyCode);
        #endif
    }

    bool Input::IsMouseButtonDown(const MouseButtonCode& mouseButtonCode)
    {
        #ifdef AW_PLATFORM_WINDOWS
            return WinInput::IsMouseButtonDown(mouseButtonCode);
        #endif
    }

}
