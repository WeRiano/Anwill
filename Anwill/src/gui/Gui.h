#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <utility>

#include "events/Event.h"
#include "events/KeyEvents.h"
#include "math/Mat4f.h"
#include "math/Algo.h"
#include "gfx/Font.h"
#include "gfx/Mesh.h"
#include "gfx/Renderer2D.h"
#include "gui/GuiElements.h"

#undef CreateWindow // MICROSOFT WOOOOOOOOOOOOOOO

namespace Anwill {

    class Gui
    {
    public:
        static void Init(const WindowSettings& ws);
        static void Render(const Timestamp& delta);
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

        static std::shared_ptr<GuiCheckbox> Checkbox(bool checkedInitially, const std::string& text,
                                                     const std::function<void(bool)>& callback,
                                                     bool onNewRow = true, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiCheckbox> Checkbox(bool checkedInitially, const std::string& text,
                                                     const std::shared_ptr<GuiContainer>& container,
                                                     const std::function<void(bool)>& callback,
                                                     bool onNewRow = true);

        static std::shared_ptr<GuiSlider<float>> Slider(float min,
                                                 float max,
                                                 float& sliderValue,
                                                 GuiWindowID windowID = 0);
        static std::shared_ptr<GuiSlider<float>> Slider(float min,
                                                 float max,
                                                 float& sliderValue,
                                                 const std::shared_ptr<GuiContainer>& container);
        static std::shared_ptr<GuiSlider<int>> Slider(int min,
                                                 int max,
                                                 int& sliderValue,
                                                 GuiWindowID windowID = 0);
        static std::shared_ptr<GuiSlider<int>> Slider(int min,
                                                 int max,
                                                 int& sliderValue,
                                                 const std::shared_ptr<GuiContainer>& container);

        static std::shared_ptr<GuiRadioButton> RadioButton(const std::string& text, int& reference,
                                                           int onSelectValue,
                                                           const std::function<void()>& callback = [](){},
                                                           bool onNewRow = true,
                                                           GuiWindowID windowID = 0);
        static std::shared_ptr<GuiRadioButton> RadioButton(const std::string& text, int& reference,
                                                           int onSelectValue,
                                                           const std::shared_ptr<GuiContainer>& container,
                                                           bool onNewRow = true,
                                                           const std::function<void()>& callback = [](){});

        static std::shared_ptr<GuiInputText> TextInput(const std::string& defaultText,
                                                         float pixelWidth,
                                                         bool onNewRow = true,
                                                         GuiWindowID windowID = 0);
        static std::shared_ptr<GuiInputText> TextInput(const std::string& defaultText,
                                                         float pixelWidth,
                                                         const std::shared_ptr<GuiContainer>& container,
                                                         bool onNewRow = true);

        static std::shared_ptr<GuiDropdown> Dropdown(const std::string& text, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiDropdown> Dropdown(const std::string& text,
                                                     const std::shared_ptr<GuiContainer>& container);

        static std::shared_ptr<GuiImage> Image(const std::string& filePath, GuiWindowID windowID = 0);
        static std::shared_ptr<GuiImage> Image(const std::string& filePath,
                                               const std::shared_ptr<GuiContainer>& container);

    private:
        static std::unique_ptr<OrthographicCamera> s_Camera;
        static GuiWindowID s_LastWindowID;
        static std::vector<std::shared_ptr<GuiWindow>> s_Windows;

        struct State {
        public:
            // Hovers
            bool hoveringDiagonalScaling = false;
            bool hoveringWindowHeader = false;
            int hoveringWindowIndex = -1;

            // Actions
            bool movingWindow = false;
            bool scalingHorizontally = false;
            bool scalingVertically = false;

            // Elements
            std::shared_ptr<GuiElement> hoverElement, pressElement, selectElement;
            Math::Vec2f hoverElementPos, pressElementPos, selectElementPos;

            // Misc
            Math::Vec2f mousePos, gameWindowSize;
        };

        static State s_State;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);
        static void OnKeyPress(std::unique_ptr<Event>& event);
        static void OnKeyRepeat(std::unique_ptr<Event>& event);
        static void OnKeyRelease(std::unique_ptr<Event>& event);
        static void OnKeyChar(std::unique_ptr<Event>& event);
        static void OnWindowResize(std::unique_ptr<Event>& event);
        static void OnGuiLoseFocus(std::unique_ptr<Event>& event);

        static void SetHoverState(const Math::Vec2f& mousePos);
        static void SetPressState();
        static void SetSelectState();
        static void ResetPressState();
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
