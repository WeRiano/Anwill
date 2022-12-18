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
#include "gui/GuiContainers.h"

#undef CreateWindow

namespace Anwill {

    class Gui
    {
    public:
        static void Init(const WindowSettings& ws);
        static void Render();

        // Client functions (API)
        static GuiWindowID CreateWindow(const std::string& title);
        static std::shared_ptr<GuiText> Text(const std::string& text,
                                             bool onNewRow = true,
                                             GuiWindowID windowID = 0);
        static std::shared_ptr<GuiText> Text(const std::string& text,
                                             const std::shared_ptr<GuiContainer>& container,
                                             bool onNewRow = true);
        static std::shared_ptr<GuiButton> Button(const std::string& text,
                                                 const std::function<void()>& callback = [](){},
                                                 bool onNewRow = true, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiButton> Button(const std::string& text,
                                                 const std::shared_ptr<GuiContainer>& container,
                                                 const std::function<void()>& callback = [](){},
                                                 bool onNewRow = true);
        static std::shared_ptr<GuiDropdown> Dropdown(const std::string& text, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiDropdown> Dropdown(const std::string& text, const std::shared_ptr<GuiContainer>& container);

    private:
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
        static void OnWindowResize(std::unique_ptr<Event>& event);

        static bool HandleHoveringAndPressing(const Math::Vec2f& mousePos);
        static bool MoveOrResizeSelectedWindow(const Math::Vec2f& newMousePos);
        static int GetWindowIndex(GuiWindowID id);

        template <class E, typename... Args>
        static std::shared_ptr<E> AddElementToWindow(GuiWindowID windowID, Args&&... args) {
            int windowIndex = GetWindowIndex(windowID);
            if(windowIndex == -1) {
                return nullptr;
            }
            return s_Windows[windowIndex].AddElement<E>(std::forward<Args>(args)...);
        }

        template <class E, typename... Args>
        static std::shared_ptr<E> AddElement(std::shared_ptr<GuiContainer> container, Args&&... args) {
            return container->AddElement<E>(std::forward<Args>(args)...);
        }
    };
}
