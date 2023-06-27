#include "core/Assert.h"
#include "core/Window.h"
#include "utils/Profiler.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"
#include "gfx/ShaderMacros.h"
#include "events/MouseEvents.h"
#include "events/WindowEvents.h"
#include "events/SystemEvents.h"

namespace Anwill {

    std::unique_ptr<OrthographicCamera> Gui::s_Camera;
    std::vector<std::shared_ptr<GuiWindow>> Gui::s_Windows;
    GuiWindowID Gui::s_LastWindowID = 0;
    Gui::State Gui::s_State;

    void Gui::Init(const WindowSettings& ws)
    {
        ShaderMacros::SetMacro("AW_GUI_WINDOW_BORDER_SIZE", GuiStyling::windowBorderSize);
        ShaderMacros::SetMacro("AW_GUI_WINDOW_HEADER_SIZE", GuiStyling::windowHeaderSize);

        s_Camera = std::make_unique<OrthographicCamera>((float) ws.width, (float) ws.height);
        GuiStyling::Init();

        SystemEvents::Subscribe<MouseMoveEvent>(OnMouseMove);
        SystemEvents::Subscribe<MouseButtonPressEvent>(OnMousePress);
        SystemEvents::Subscribe<MouseButtonReleaseEvent>(OnMouseRelease);
        SystemEvents::Subscribe<WindowResizeEvent>(OnWindowResize);
    }

    void Gui::Render()
    {
        if(s_Windows.empty()) {
            return;
        }
        AW_PROFILE_FUNC();
        Renderer2D::BeginScene(*s_Camera);
        bool last = false;
        // Render from back to front and highlight the front window as selected
        for(int i = s_Windows.size() - 1; i >= 0; i--) {
            if(i == 0) {
                last = true;
            }
            s_Windows[i]->Render(last);
        }
    }

    void Gui::Update()
    {
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
        return AddElementToWindow<GuiText>(windowID, onNewRow, false, text, GuiStyling::fontSize);
    }

    std::shared_ptr<GuiText> Gui::Text(const std::string& text,
                                       const std::shared_ptr<GuiContainer>& container,
                                       bool onNewRow)
    {
        return container->AddElement<GuiText>(onNewRow, false, text, GuiStyling::fontSize);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text, const std::function<void()>& callback, bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiTextButton>(windowID, onNewRow, false, text, GuiStyling::fontSize, callback);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container,
                                               const std::function<void()>& callback,
                                               bool onNewRow)
    {
        return container->AddElement<GuiTextButton>(onNewRow, false, text, GuiStyling::fontSize, callback);
    }

    std::shared_ptr<GuiCheckbox> Gui::Checkbox(bool checkedInitially,
                                               const std::function<void(bool)> &callback,
                                               bool onNewRow,
                                               GuiWindowID windowID) {
        return AddElementToWindow<GuiCheckbox>(windowID, onNewRow, false, checkedInitially, callback);
    }

    std::shared_ptr<GuiCheckbox> Gui::Checkbox(bool checkedInitially, const std::shared_ptr<GuiContainer> &container,
                                               const std::function<void(bool)> &callback, bool onNewRow) {
        return container->AddElement<GuiCheckbox>(onNewRow, false, checkedInitially, callback);
    }

    std::shared_ptr<GuiSlider> Gui::Slider(float min, float max, float* sliderValue, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiFloatSlider>(windowID, true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiSlider>
    Gui::Slider(float min, float max, float* sliderValue, const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiFloatSlider>(true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiSlider> Gui::Slider(int min, int max, int* sliderValue, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiIntSlider>(windowID, true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiSlider> Gui::Slider(int min, int max, int* sliderValue,
                                           const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiIntSlider>(true, true, min, max, sliderValue);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiDropdown>(windowID, true, true, text,
                                               GuiStyling::fontSize);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiDropdown>(true, true, text, GuiStyling::fontSize);
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
        if(!MoveOrResizeSelectedWindow(newMousePos)) {
            // We are NOT resizing or moving a window, so we change the hover state
            SetHoverState(newMousePos);
        }
        s_State.mousePos = newMousePos;
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        if(s_State.hoveringWindowIndex == -1) { return; }
        if(s_State.hoveringWindowIndex != 0) {
            // Select window if we are hovering a window and the window is not already selected
            std::rotate(s_Windows.begin(),
                        s_Windows.begin() + s_State.hoveringWindowIndex, s_Windows.begin() + s_State.hoveringWindowIndex + 1);
            s_State.hoveringWindowIndex = 0;
        }
        if(s_State.hoveringWindowHeader) {
            s_State.movingWindow = true;
        }
        if(s_State.hoveringDiagonalScaling) {
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

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        s_State.movingWindow = false;
        s_State.scalingHorizontally = false;
        s_State.scalingVertically = false;
        if(s_State.pressElement != nullptr) {
            s_State.pressElement->Release();
            s_State.pressElement = nullptr;
        }
    }

    void Gui::OnWindowResize(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<WindowResizeEvent&>(*event);
        s_State.gameWindowSize = { (float) e.GetNewWidth(), (float) e.GetNewHeight() };
        s_Camera->SetProjection((float) e.GetNewWidth(), (float) e.GetNewHeight());
    }

    void Gui::SetHoverState(const Math::Vec2f& mousePos)
    {
        bool lastIterHoveringDiagonalScaling = s_State.hoveringDiagonalScaling;
        bool lastIterHoveringHeader = s_State.hoveringWindowHeader;
        for (int i = 0; i < s_Windows.size(); i++)
        {
            if(!s_Windows[i]->IsHidingElements() && s_Windows[i]->IsHoveringResize(s_State.mousePos)) {
                if(!lastIterHoveringDiagonalScaling) {
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::NegativeDiagonalResize);
                    s_State.hoveringDiagonalScaling = true;
                }
                s_State.hoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringHeader(s_State.mousePos)) {
                if(!lastIterHoveringHeader) {
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::GrabbingHand);
                    s_State.hoveringWindowHeader = true;
                }
                s_State.hoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringWindow(mousePos)) {
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
        SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);

        if(s_State.hoverElement != nullptr) {
            // If we did not hit a window, but we are hovering something from last iteration we stop hovering
            // that
            s_State.hoverElement->StopHovering();
            s_State.hoverElement = nullptr;
        }
    }

    bool Gui::MoveOrResizeSelectedWindow(const Math::Vec2f& newMousePos)
    {
        if(s_Windows.empty()) {
            return false;
        }

        Math::Vec2f mouseDelta = newMousePos - s_State.mousePos;
        Math::Vec2f maxPos = s_State.gameWindowSize;
        if(s_State.movingWindow) {
            s_Windows[0]->Move(mouseDelta, {0.0f, 0.0f}, maxPos);
            return true;
        } else if(s_State.scalingHorizontally || s_State.scalingVertically)
        {
            Math::Vec2f windowPos = s_Windows[0]->GetPos();
            s_Windows[0]->Resize({mouseDelta.GetX(), -mouseDelta.GetY()}, {100.0f, 100.0f},
                                {maxPos.GetX() - windowPos.GetX(), windowPos.GetY()});
            return true;
        }
        return false;
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