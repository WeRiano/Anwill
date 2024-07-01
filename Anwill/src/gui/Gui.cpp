#include "core/Assert.h"
#include "core/Window.h"
#include "core/Timestamp.h"
#include "utils/Profiler.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"
#include "events/GuiEventHandler.h"
#include "events/MouseEvents.h"
#include "events/WindowEvents.h"
#include "events/SystemEventHandler.h"

namespace Anwill {

    std::unique_ptr<OrthographicCamera> Gui::s_Camera;
    std::vector<std::shared_ptr<GuiWindow>> Gui::s_Windows;
    GuiWindowID Gui::s_LastWindowID = 0;
    Gui::State Gui::s_State;

    void Gui::Init(const WindowSettings& ws)
    {
        s_Camera = std::make_unique<OrthographicCamera>((float) ws.width, (float) ws.height);
        s_State.gameWindowSize = {(float) ws.width, (float) ws.height};

        GuiStyling::InitGlobalStyling();
        GuiEventHandler::Init();

        SystemEventHandler::Subscribe<MouseMoveEvent>(OnMouseMove);
        SystemEventHandler::Subscribe<MouseButtonPressEvent>(OnMousePress);
        SystemEventHandler::Subscribe<MouseButtonReleaseEvent>(OnMouseRelease);
        SystemEventHandler::Subscribe<MouseScrollEvent>(OnMouseScroll);
        SystemEventHandler::Subscribe<KeyPressEvent>(OnKeyPress);
        SystemEventHandler::Subscribe<KeyRepeatEvent>(OnKeyRepeat);
        SystemEventHandler::Subscribe<KeyReleaseEvent>(OnKeyRelease);
        SystemEventHandler::Subscribe<KeyCharEvent>(OnKeyChar);
        SystemEventHandler::Subscribe<WindowResizeEvent>(OnWindowResize);

        GuiEventHandler::Subscribe<GuiLoseFocusEvent>(OnGuiLoseFocus);
    }

    void Gui::Render(const Timestamp& delta)
    {
        if(s_Windows.empty()) {
            return;
        }
        AW_PROFILE_FUNC();
        Renderer2D::BeginScene(*s_Camera);
        // Render from back to front and highlight the front window as selected
        for(int i = s_Windows.size() - 1; i >= 0; i--) {
            bool isLastElement = i == 0;
            s_Windows[i]->Render(isLastElement, delta);
        }

        if(s_State.hoverElement != nullptr)
        {
            s_State.hoverElement->OnHoverRender(s_State.mousePos,
                                                s_State.gameWindowSize);
        }
    }

    void Gui::Update()
    {
        AW_PROFILE_FUNC();
        GuiEventHandler::Pop();
        if(s_State.hoverElement != nullptr)
        {
            s_State.hoverElement->OnHover(s_State.mousePos - s_State.hoverElementPos);
        }
        if(s_State.pressElement != nullptr)
        {
            s_State.pressElement->OnPress(s_State.mousePos - s_State.pressElementPos);
        }
    }

    std::shared_ptr<GuiText> Gui::Text(const std::string& text, bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiText>(windowID, onNewRow, false, text, GuiStyling::Text::fontSize);
    }

    std::shared_ptr<GuiText> Gui::Text(const std::string& text,
                                       const std::shared_ptr<GuiContainer>& container,
                                       bool onNewRow)
    {
        return container->AddElement<GuiText>(onNewRow, false, text, GuiStyling::Text::fontSize);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text,
                                               const std::function<void()>& callback,
                                               bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiTextButton>(windowID, onNewRow, false, text,
                                                 GuiStyling::Text::fontSize, callback);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container,
                                               const std::function<void()>& callback,
                                               bool onNewRow)
    {
        return container->AddElement<GuiTextButton>(onNewRow, false, text,
                                                    GuiStyling::Text::fontSize, callback);
    }

    std::shared_ptr<GuiCheckbox> Gui::Checkbox(bool checkedInitially, const std::string& text,
                                               const std::function<void(bool)>& callback,
                                               bool onNewRow,
                                               GuiWindowID windowID) {
        return AddElementToWindow<GuiCheckbox>(windowID, onNewRow, false, checkedInitially, text,
                                               GuiStyling::Text::fontSize, callback);
    }

    std::shared_ptr<GuiCheckbox> Gui::Checkbox(bool checkedInitially, const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container,
                                               const std::function<void(bool)>& callback, bool onNewRow) {
        return container->AddElement<GuiCheckbox>(onNewRow, false, checkedInitially, text,
                                                  GuiStyling::Text::fontSize, callback);
    }

    std::shared_ptr<GuiSlider<float>> Gui::Slider(float min, float max, float& sliderValue,
                                                  GuiWindowID windowID)
    {
        return AddElementToWindow<GuiSlider<float>>(windowID, true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiSlider<float>> Gui::Slider(float min, float max, float& sliderValue,
                                                  const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiSlider<float>>(true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiSlider<int>> Gui::Slider(int min, int max, int& sliderValue, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiSlider<int>>(windowID, true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiSlider<int>> Gui::Slider(int min, int max, int& sliderValue,
                                           const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiSlider<int>>(true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiRadioButton> Gui::RadioButton(const std::string& text,
                                                     int& reference,
                                                     int onSelectValue,
                                                     const std::function<void()>& callback,
                                                     bool onNewRow,
                                                     GuiWindowID windowID)
    {
        return AddElementToWindow<GuiRadioButton>(windowID, onNewRow, false, text, GuiStyling::Text::fontSize,
                                                  reference, onSelectValue, callback);
    }

    std::shared_ptr<GuiRadioButton> Gui::RadioButton(const std::string& text,
                                                     int& reference,
                                                     int onSelectValue,
                                                     const std::shared_ptr<GuiContainer>& container,
                                                     bool onNewRow,
                                                     const std::function<void()>& callback)
    {
        return container->AddElement<GuiRadioButton>(onNewRow, false, text, GuiStyling::Text::fontSize,
                                                   reference, onSelectValue, callback);
    }

    std::shared_ptr<GuiInputText> Gui::TextInput(const std::string& defaultText, float pixelWidth, bool onNewRow,
                                                 GuiWindowID windowID)
    {
        return AddElementToWindow<GuiInputText>(windowID, onNewRow, false, defaultText,
                                                GuiStyling::Text::fontSize, pixelWidth);
    }

    std::shared_ptr<GuiInputText> Gui::TextInput(const std::string& defaultText, float pixelWidth,
                                                 const std::shared_ptr<GuiContainer>& container,
                                                 bool onNewRow)
    {
        return container->AddElement<GuiInputText>(onNewRow, false, defaultText, GuiStyling::Text::fontSize,
                                                   pixelWidth);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiDropdown>(windowID, true, true, text,
                                               GuiStyling::Text::fontSize);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiDropdown>(true, true, text, GuiStyling::Text::fontSize);
    }

    std::shared_ptr<GuiImage> Gui::Image(const std::string &filePath, unsigned int maxRows,
                                         GuiWindowID windowID)
    {
        return AddElementToWindow<GuiImage>(windowID, true, true, filePath, maxRows);
    }

    std::shared_ptr<GuiImage> Gui::Image(const std::string &filePath, unsigned int maxRows,
                                         const std::shared_ptr<GuiContainer> &container)
    {
        return container->AddElement<GuiImage>(true, true, filePath, maxRows);
    }

    GuiWindowID Gui::CreateWindow(const std::string& title)
    {
        s_LastWindowID++;
        s_Windows.emplace_back(std::make_shared<GuiWindow>(title, s_LastWindowID, Math::Vec2f(0.0f, 900.0f),
                               Math::Vec2f(600.0f, 450.0f)));
        return s_LastWindowID;
        // TODO: Max nr of windows (id cap or something)
    }

    void Gui::OnMouseMove(std::unique_ptr<Event>& event)
    {
        AW_PROFILE_FUNC();
        auto e = static_cast<MouseMoveEvent&>(*event);
        Math::Vec2f newMousePos = {e.GetXPos(), e.GetYPos()};
        Math::Vec2f mouseDelta = newMousePos - s_State.mousePos;
        Math::Vec2f maxPos = s_State.gameWindowSize;

        if(s_State.movingWindow) {
            // If state is set to move window, we move the window.
            s_Windows.front()->Move(mouseDelta, {0.0f, 0.0f}, maxPos);
        } else if(s_State.scalingHorizontally || s_State.scalingVertically)
        {
            // If state is set to scale window, we scale window.
            Math::Vec2f windowPos = s_Windows.front()->GetPos();
            s_Windows.front()->Resize({mouseDelta.X, -mouseDelta.Y}, {000.0f, 000.0f},
                                 {maxPos.X - windowPos.X, windowPos.Y});
        } else {
            // Otherwise we update the hover state.
            SetHoverState(newMousePos);
        }
        s_State.mousePos = newMousePos;
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        SetPressState();
        SetSelectState();
    }

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        ResetPressState();
    }

    void Gui::OnMouseScroll(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<MouseScrollEvent&>(*event);
        MouseScrollCode mouseScrollCode = e.GetScrollCode();
        switch(mouseScrollCode) {
            case MouseScrollCode::Up:
                s_Windows.front()->ScrollUp();
                break;
            case MouseScrollCode::Down:
                s_Windows.front()->ScrollDown();
                break;
        }
    }

    void Gui::OnKeyPress(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<KeyPressEvent&>(*event);
        if(s_State.selectElement != nullptr)
            s_State.selectElement->OnKeyPress(e.GetKeyCode());
    }

    void Gui::OnKeyRepeat(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<KeyRepeatEvent&>(*event);
        if(s_State.selectElement != nullptr)
            s_State.selectElement->OnKeyRepeat(e.GetKeyCode());
    }

    void Gui::OnKeyRelease(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<KeyReleaseEvent&>(*event);
        if(s_State.selectElement != nullptr)
            s_State.selectElement->OnKeyRelease(e.GetKeyCode());
    }

    void Gui::OnKeyChar(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<KeyCharEvent&>(*event);
        if(s_State.selectElement != nullptr)
            s_State.selectElement->OnKeyChar(e.GetChar());
    }

    void Gui::OnWindowResize(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<WindowResizeEvent&>(*event);
        s_State.gameWindowSize = { (float) e.GetNewWidth(), (float) e.GetNewHeight() };
        s_Camera->SetProjection((float) e.GetNewWidth(), (float) e.GetNewHeight());
    }

    void Gui::OnGuiLoseFocus(std::unique_ptr<Event>& event)
    {
        s_State.selectElement->Deselect();
        s_State.selectElement = nullptr;
    }

    void Gui::SetHoverState(const Math::Vec2f& mousePos)
    {
        // TODO: What the f is this function
        bool lastIterHoveringDiagonalScaling = s_State.hoveringDiagonalScaling;
        bool lastIterHoveringHeader = s_State.hoveringWindowHeader;
        for (int i = 0; i < s_Windows.size(); i++)
        {
            if(!s_Windows[i]->IsHidingElements() && s_Windows[i]->IsHoveringResize(s_State.mousePos)) {
                if(!lastIterHoveringDiagonalScaling) {
                    SystemEventHandler::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::NegativeDiagonalResize);
                    s_State.hoveringDiagonalScaling = true;
                }
                s_State.hoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringHeader(s_State.mousePos)) {
                if(!lastIterHoveringHeader) {
                    SystemEventHandler::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::GrabbingHand);
                    s_State.hoveringWindowHeader = true;
                }
                s_State.hoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringWindow(mousePos)) {
                SystemEventHandler::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);
                // Update which window we are currently hovering
                s_State.hoveringWindowIndex = i;
                // Remember which element we hovered last iteration
                std::shared_ptr<GuiElement> lastIterHoverElement = s_State.hoverElement;
                // Update which element we are hovering right now
                s_State.hoverElement = s_Windows[i]->GetHoverElement(s_State.hoverElementPos,
                                                                     mousePos);
                if(s_State.hoverElement != lastIterHoverElement) {
                    if(s_State.hoverElement != nullptr) {
                        s_State.hoverElement->StartHovering();
                    }
                    if(lastIterHoverElement != nullptr) {
                        lastIterHoverElement->StopHovering();
                    }
                }
                // We can only hover 1 window and 1 element so no need to continue.
                s_State.hoveringDiagonalScaling = false;
                s_State.hoveringWindowHeader = false;
                return;
            }
        }

        // We are not hovering anything at this point
        s_State.hoveringDiagonalScaling = false;
        s_State.hoveringWindowHeader = false;
        s_State.hoveringWindowIndex = -1;
        SystemEventHandler::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);

        if(s_State.hoverElement != nullptr) {
            // If we did not hit a window,
            // but we are hovering something from last iteration, we stop hovering
            s_State.hoverElement->StopHovering();
            s_State.hoverElement = nullptr;
        }
    }

    void Gui::SetPressState()
    {
        if(s_State.hoveringWindowIndex == -1) {
            // We are not hovering a window at all.
            return;
        }
        if(s_State.hoveringWindowIndex != 0) {
            // We are hovering a window that is not the selected window.
            // Select the window by moving it to the front.
            std::rotate(s_Windows.begin(),
                        s_Windows.begin() + s_State.hoveringWindowIndex,
                        s_Windows.begin() + s_State.hoveringWindowIndex + 1);
            s_State.hoveringWindowIndex = 0;
        }
        if(s_State.hoveringWindowHeader) {
            // We are hovering the header.
            s_State.movingWindow = true;
        } else if(s_State.hoveringDiagonalScaling) {
            // We are hovering the resize.
            s_State.scalingHorizontally = true;
            s_State.scalingVertically = true;
        } else
        {
            if(s_State.hoverElement != nullptr) {
                s_State.pressElement = s_State.hoverElement;
                s_State.pressElementPos = s_State.hoverElementPos;
                s_State.pressElement->StartPressing();
            }
        }
    }

    void Gui::SetSelectState()
    {
        if(s_State.selectElement != s_State.hoverElement) {
            if (s_State.selectElement != nullptr) {
                s_State.selectElement->Deselect();
            }
            s_State.selectElement = s_State.hoverElement;
            if(s_State.selectElement != nullptr) {
                s_State.selectElement->Select();
            }
        }
    }

    void Gui::ResetPressState()
    {
        s_State.movingWindow = false;
        s_State.scalingHorizontally = false;
        s_State.scalingVertically = false;
        if(s_State.pressElement != nullptr) {
            s_State.pressElement->Release();
            s_State.pressElement = nullptr;
        }
    }

    int Gui::GetWindowIndex(GuiWindowID id)
    {
        if(s_Windows.empty()) {
            AW_ERROR("There are no active windows.");
            return -1;
        }
        if(id == 0) {
            return 0;
        } else {
            for(int i = 0; i < s_Windows.size(); i++) {
                if (s_Windows[i]->GetID() == id) {
                    return i;
                }
            }
        }
        AW_ERROR("The window you requested does not exist.");
        return -1;
    }
}