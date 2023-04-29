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
        ShaderMacros::SetMacro("AW_GUI_WINDOW_BORDER_SIZE", GuiMetrics::WindowBorderSize);
        ShaderMacros::SetMacro("AW_GUI_WINDOW_HEADER_SIZE", GuiMetrics::WindowHeaderSize);

        s_Camera = std::make_unique<OrthographicCamera>((float) ws.width, (float) ws.height);
        GuiMetrics::windowSize = { (float) ws.width, (float) ws.height };
        GuiElement::s_Font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        GuiElement::s_RectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        GuiElement::s_TriangleMesh = Mesh::CreateTriangleMesh({0.0f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f});
        GuiElement::s_PrimitiveShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiPrimitiveColor.glsl");
        GuiWindow::s_WindowShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");
        GuiText::s_Shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiText.glsl");
        GuiTextButton::s_Shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiTextButton.glsl");
        GuiCheckbox::s_CheckmarkMesh = Mesh::CreateCheckmarkMesh(1.0f, 1.0f);

        GuiElement::s_PrimitiveShader->Bind();
        GuiElement::s_PrimitiveShader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
        GuiElement::s_PrimitiveShader->Unbind();

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
        if(s_State.HoverElement != nullptr)
        {
            s_State.HoverElement->OnHover(s_State.MousePos - s_State.HoverElementPos);
        }
        if(s_State.PressElement != nullptr)
        {
            s_State.PressElement->OnPress(s_State.MousePos - s_State.PressElementPos);
        }
    }

    std::shared_ptr<GuiText> Gui::Text(const std::string& text, bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiText>(windowID, onNewRow, false, text, GuiMetrics::FontSize);
    }

    std::shared_ptr<GuiText> Gui::Text(const std::string& text,
                                       const std::shared_ptr<GuiContainer>& container,
                                       bool onNewRow)
    {
        return container->AddElement<GuiText>(onNewRow, false, text, GuiMetrics::FontSize);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text, const std::function<void()>& callback, bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiTextButton>(windowID, onNewRow, false, text, GuiMetrics::FontSize, callback);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container,
                                               const std::function<void()>& callback,
                                               bool onNewRow)
    {
        return container->AddElement<GuiTextButton>(onNewRow, false, text, GuiMetrics::FontSize, callback);
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

    std::shared_ptr<GuiSlider> Gui::Slider(float min, float max, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiSlider>(windowID, true, true, min, max);
    }

    std::shared_ptr<GuiSlider>
    Gui::Slider(float min, float max, const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiSlider>(true, true, min, max);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiDropdown>(windowID, true, true, text, GuiMetrics::FontSize);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiDropdown>(true, true, text, GuiMetrics::FontSize);
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
        s_State.MousePos = newMousePos;
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        if(s_State.HoveringWindowIndex == -1) { return; }
        if(s_State.HoveringWindowIndex != 0) {
            // Select window if we are hovering a window and the window is not already selected
            std::rotate(s_Windows.begin(),
                        s_Windows.begin() + s_State.HoveringWindowIndex, s_Windows.begin() + s_State.HoveringWindowIndex + 1);
            s_State.HoveringWindowIndex = 0;
        }
        if(s_State.HoveringWindowHeader) {
            s_State.MovingWindow = true;
        }
        if(s_State.HoveringDiagonalScaling) {
            s_State.ScalingHorizontally = true;
            s_State.ScalingVertically = true;
        } else
        {
            if(s_State.HoverElement != nullptr) {
                s_State.PressElement = s_State.HoverElement;
                s_State.PressElementPos = s_State.HoverElementPos;
                s_State.PressElement->StartPressing();
            }
        }
    }

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        s_State.MovingWindow = false;
        s_State.ScalingHorizontally = false;
        s_State.ScalingVertically = false;
        if(s_State.PressElement != nullptr) {
            s_State.PressElement->Release();
            s_State.PressElement = nullptr;
        }
    }

    void Gui::OnWindowResize(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<WindowResizeEvent&>(*event);
        GuiMetrics::windowSize = { (float) e.GetNewWidth(), (float) e.GetNewHeight() };
        s_Camera->SetProjection((float) e.GetNewWidth(), (float) e.GetNewHeight());
    }

    void Gui::SetHoverState(const Math::Vec2f& mousePos)
    {
        // We just "reset" mouse to arrow each loop now. Prob slower but w/e for now
        SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);

        bool lastIterHoveringDiagonalScaling = s_State.HoveringDiagonalScaling;
        bool lastIterHoveringHeader = s_State.HoveringWindowHeader;
        for (int i = 0; i < s_Windows.size(); i++)
        {
            if(!s_Windows[i]->IsHidingElements() && s_Windows[i]->IsHoveringResize(s_State.MousePos)) {
                if(!lastIterHoveringDiagonalScaling) {
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::NegativeDiagonalResize);
                    s_State.HoveringDiagonalScaling = true;
                }
                s_State.HoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringHeader(s_State.MousePos)) {
                if(!lastIterHoveringHeader) {
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::GrabbingHand);
                    s_State.HoveringWindowHeader = true;
                }
                s_State.HoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringWindow(mousePos)) {
                // Update which window we are currently hovering
                s_State.HoveringWindowIndex = i;
                // Remember which element we hovered last iteration
                std::shared_ptr<GuiElement> lastIterHoverElement = s_State.HoverElement;
                // Update which element we are hovering right now
                s_State.HoverElement = s_Windows[i]->GetHoverElement(s_State.HoverElementPos,
                                                                     mousePos);
                if(s_State.HoverElement != lastIterHoverElement) {
                    if(s_State.HoverElement != nullptr) {
                        s_State.HoverElement->StartHovering();
                    }
                    if(lastIterHoverElement != nullptr) {
                        lastIterHoverElement->StopHovering();
                    }
                }
                // We can only hover 1 window and 1 element so no need to continue.
                s_State.HoveringDiagonalScaling = false;
                s_State.HoveringWindowHeader = false;
                return;
            }
        }

        // We are not hovering anything at this point
        s_State.HoveringDiagonalScaling = false;
        s_State.HoveringWindowHeader = false;
        s_State.HoveringWindowIndex = -1;
        if(s_State.HoverElement != nullptr) {
            // If we did not hit a window, but we are hovering something from last iteration we stop hovering that
            s_State.HoverElement->StopHovering();
            s_State.HoverElement = nullptr;
        }
    }

    bool Gui::MoveOrResizeSelectedWindow(const Math::Vec2f& newMousePos)
    {
        if(s_Windows.empty()) {
            return false;
        }

        Math::Vec2f mouseDelta = newMousePos - s_State.MousePos;
        Math::Vec2f maxPos = GuiMetrics::windowSize;
        if(s_State.MovingWindow) {
            s_Windows[0]->Move(mouseDelta, {0.0f, 0.0f}, maxPos);
            return true;
        } else if(s_State.ScalingHorizontally || s_State.ScalingVertically)
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