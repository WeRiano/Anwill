#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <utility>

#include "events/Event.h"
#include "math/Mat4f.h"
#include "math/Algo.h"
#include "gfx/Font.h"
#include "gfx/Mesh.h"
#include "gfx/Renderer2D.h"
#include "gui/GuiElements.h"

#undef CreateWindow

namespace Anwill {

    class Gui
    {
    public:
        static void Init(const WindowSettings& ws);
        static void Render();

        // Client functions (API)
        static GuiWindowID CreateWindow(const std::string& title);
        static std::shared_ptr<GuiText> Text(const std::string& text, bool newRow = true, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiButton> Button(const std::string& text, const std::function<void()>& callback = [](){}, bool newRow = true, GuiWindowID windowID = 0);

    private:
        static WindowSettings s_WindowSettings;
        static std::unique_ptr<OrthographicCamera> s_Camera;
        static GuiWindowID s_NextID;
        static std::vector<GuiWindow> s_Windows;

        // Gui state stuff
        static Math::Vec2f s_MousePos;
        static bool s_Moving, s_ScalingX, s_ScalingY;
        static bool s_HoveringDiagonalScaling, s_HoveringHeader;
        static int s_HoveringWindowIndex;
        static std::shared_ptr<GuiElement> s_HoverElement;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);

        static bool SetMovingOrResizing();
        static bool UpdateSelectedWindow(const Math::Vec2f& newMousePos);
        static int GetWindowIndex(GuiWindowID id);
    };
}
