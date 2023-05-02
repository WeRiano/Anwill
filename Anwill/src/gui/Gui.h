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
        static void Update();

        // Client functions (API)
        static GuiWindowID CreateWindow(const std::string& title);
        static std::shared_ptr<GuiText> Text(const std::string& text,
                                             bool onNewRow = true,
                                             GuiWindowID windowID = 0);
        static std::shared_ptr<GuiText> Text(const std::string& text,
                                             const std::shared_ptr<GuiContainer>& container,
                                             bool onNewRow = true);
        static std::shared_ptr<GuiTextButton> Button(const std::string& text,
                                                     const std::function<void()>& callback = [](){},
                                                     bool onNewRow = true, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiTextButton> Button(const std::string& text,
                                                     const std::shared_ptr<GuiContainer>& container,
                                                     const std::function<void()>& callback = [](){},
                                                     bool onNewRow = true);
        static std::shared_ptr<GuiCheckbox> Checkbox(bool checkedInitially,
                                                     const std::function<void(bool)>& callback = [](bool checked){},
                                                     bool onNewRow = true, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiCheckbox> Checkbox(bool checkedInitially,
                                                     const std::shared_ptr<GuiContainer>& container,
                                                     const std::function<void(bool)>& callback = [](bool checked){},
                                                     bool onNewRow = true);
        static std::shared_ptr<GuiSlider> Slider(float min,
                                                 float max,
                                                 GuiWindowID windowID = 0);
        static std::shared_ptr<GuiSlider> Slider(float min,
                                                 float max,
                                                 const std::shared_ptr<GuiContainer>& container);
        static std::shared_ptr<GuiSlider> Slider(int min,
                                                 int max,
                                                 GuiWindowID windowID = 0);
        static std::shared_ptr<GuiSlider> Slider(int min,
                                                 int max,
                                                 const std::shared_ptr<GuiContainer>& container);
        static std::shared_ptr<GuiDropdown> Dropdown(const std::string& text, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiDropdown> Dropdown(const std::string& text, const std::shared_ptr<GuiContainer>& container);

    private:
        static std::unique_ptr<OrthographicCamera> s_Camera;
        static GuiWindowID s_LastWindowID;
        static std::vector<std::shared_ptr<GuiWindow>> s_Windows;

        struct State {
        public:
            // Hovers
            bool HoveringDiagonalScaling = false;
            bool HoveringWindowHeader = false;
            int HoveringWindowIndex = -1;

            // Actions
            bool MovingWindow = false;
            bool ScalingHorizontally = false;
            bool ScalingVertically = false;

            // Elements
            std::shared_ptr<GuiElement> HoverElement, PressElement;
            Math::Vec2f HoverElementPos, PressElementPos;
            Math::Vec2f MousePos;
        };

        static State s_State;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);
        static void OnWindowResize(std::unique_ptr<Event>& event);

        static void SetHoverState(const Math::Vec2f& mousePos);
        static bool MoveOrResizeSelectedWindow(const Math::Vec2f& newMousePos);
        static int GetWindowIndex(GuiWindowID id);

        template <class E, typename... Args>
        static std::shared_ptr<E> AddElementToWindow(GuiWindowID windowID, Args&&... args) {
            int windowIndex = GetWindowIndex(windowID);
            if(windowIndex == -1) {
                return nullptr;
            }
            return s_Windows[windowIndex]->AddElement<E>(std::forward<Args>(args)...);
        }
    };
}
