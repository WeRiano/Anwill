#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>

#include "events/Event.h"
#include "math/Mat4f.h"
#include "math/Algo.h"
#include "gfx/Font.h"
#include "gfx/Mesh.h"
#include "gfx/Renderer2D.h"
#include "gui/GuiElements.h"

namespace Anwill {

    class Gui
    {
    public:
        static void Init(const WindowSettings& ws);
        static void Render();

        // Client functions (API)
        static GuiWindowID NewWindow(const std::string& title);
        static void Text(const std::string& text, bool newRow, GuiWindowID windowID = 0);
        static void Button(const std::string& text, const std::function<void()>& callback, bool newRow, GuiWindowID windowID = 0);

    private:
        static std::unique_ptr<OrthographicCamera> s_Camera;
        static std::vector<GuiWindow> s_Windows;
        static GuiWindowID s_NextID;

        static WindowSettings s_WindowSettings;
        static Math::Vec2f s_MousePos;
        static bool s_Moving, s_ScalingX, s_ScalingY;
        static bool s_HoveringDiagonalScaling, s_HoveringHeader;
        static int s_HoveringWindow;
        static std::shared_ptr<GuiElement> s_HoverElement;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);

        static bool SetMovingOrResizing();
        static bool UpdateSelectedWindow(const Math::Vec2f& newMousePos);
        static GuiWindow& GetWindow(GuiWindowID id);

        template <class E, typename... Args>
        static void AddHorizontalElement(GuiWindowID windowID, Args&&... args) {
            if(s_Windows.empty()) {
                AW_ERROR("There are currently no active GUI windows.");
                return;
            }
            GetWindow(windowID).AddHorizontalElement<E>(std::forward<Args>(args)...);
        }

        template <class E, typename... Args>
        static void AddVerticalElement(GuiWindowID windowID, Args&&... args) {
            if(s_Windows.empty()) {
                AW_ERROR("There are currently no active GUI windows.");
                return;
            }
            GetWindow(windowID).AddVerticalElement<E>(std::forward<Args>(args)...);
        }
    };
}
