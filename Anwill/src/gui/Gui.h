#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <utility>

#include "elements/GuiButton.h"
#include "elements/GuiCheckbox.h"
#include "elements/GuiDropdown.h"
#include "elements/GuiElement.h"
#include "elements/GuiIcon.h"
#include "elements/GuiImage.h"
#include "elements/GuiInputText.h"
#include "elements/GuiRadioButton.h"
#include "elements/GuiSlider.h"
#include "elements/GuiText.h"
#include "elements/GuiTextbutton.h"
#include "elements/GuiTooltip.h"

#include "containers/GuiWindow.h"

#include "events/Event.h"
#include "events/KeyEvents.h"
#include "math/Mat4f.h"
#include "math/Algo.h"
#include "gfx/Font.h"
#include "gfx/Mesh.h"
#include "gfx/Renderer2D.h"

#undef CreateWindow // MICROSOFT WOOOOOOOOOOOOOOO

#define AW_GUI_MAX_NUM_WINDOWS 5

namespace Anwill {

    class Gui
    {
    public:
        // Client functions (API)
        static Shared<GuiWindow> CreateWindow(const std::string& title);

        static Shared<GuiDropdown> Dropdown(const std::string& title,
                                                     const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiText> Text(const std::string& text,
                                             bool onNewRow = true,
                                             const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiTextButton> Button(const std::string& text,
                                                     const std::function<void()>& callback,
                                                     bool onNewRow = true,
                                                     const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiCheckbox> Checkbox(bool checkedInitially, const std::string& text,
                                                     const std::function<void(bool)>& callback,
                                                     bool onNewRow = true,
                                                     const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiSlider<float>> Slider(float min,
                                                 float max,
                                                 float& sliderValue,
                                                 bool onNewRow = true,
                                                 const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiSlider<int>> Slider(int min,
                                                      int max,
                                                      int& sliderValue,
                                                      bool onNewRow = true,
                                                      const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiRadioButton> RadioButton(const std::string& text, int& reference,
                                                           int onSelectValue,
                                                           const std::function<void()>& callback,
                                                           bool onNewRow = true,
                                                   const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiInputText> TextInput(const std::string& startText,
                                                       float pixelWidth,
                                                       bool onNewRow = true,
                                                   const Shared<GuiContainer>& container = nullptr);

        static Shared<GuiImage> Image(const std::string& filePath, unsigned int maxRows,
                                               const Shared<GuiContainer>& container = nullptr);

        static void Init(const WindowSettings& ws);
        static void Render(const Timestamp& delta);
        static void Update();

    private:
        static std::unique_ptr<OrthographicCamera> s_Camera;
        static Shared<GuiContainer> s_LastContainer;
        //static GuiWindowID s_LastWindowID;
        static std::vector<Shared<GuiWindow>> s_Windows;
        // API remembers last used container

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
            Shared<GuiElement> hoverElement, pressElement, selectElement;
            Math::Vec2f hoverElementPos, pressElementPos, selectElementPos;

            // Misc
            Math::Vec2f mousePos, gameWindowSize;
        };

        static State s_State;

        static void OnMouseMove(std::unique_ptr<Event>& event);
        static void OnMousePress(std::unique_ptr<Event>& event);
        static void OnMouseRelease(std::unique_ptr<Event>& event);
        static void OnMouseScroll(std::unique_ptr<Event>& event);
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
        //static int GetWindowIndex(GuiWindowID id);

        template <class E, typename... Args>
        static Shared<E> AddElementToContainer(const Shared<GuiContainer>& container,
                                                        Args&&... args)
        {
            if(container == nullptr && s_LastContainer == nullptr)
            {
                AW_ERROR("Create a window before adding elements.");
                return nullptr;
            }
            s_LastContainer = (container != nullptr) ? container : s_LastContainer;
            return s_LastContainer->AddElement<E>(std::forward<Args>(args)...);
        }
    };
}
