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
    Math::Vec2f Gui::s_MousePos;
    bool Gui::s_Moving = false;
    bool Gui::s_ScalingX = false;
    bool Gui::s_ScalingY = false;
    bool Gui::s_HoveringDiagonalScaling = false;
    bool Gui::s_HoveringHeader = false;
    int Gui::s_HoveringWindowIndex;
    std::shared_ptr<GuiElement> Gui::s_HoverElement;

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

    std::shared_ptr<GuiText> Gui::Text(const std::string& text, bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiText>(windowID, onNewRow, text, GuiMetrics::FontSize);
    }

    std::shared_ptr<GuiText> Gui::Text(const std::string& text,
                                       const std::shared_ptr<GuiContainer>& container,
                                       bool onNewRow)
    {
        return container->AddElement<GuiText>(onNewRow, text, GuiMetrics::FontSize);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text, const std::function<void()>& callback, bool onNewRow, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiTextButton>(windowID, onNewRow, text, GuiMetrics::FontSize, callback);
    }

    std::shared_ptr<GuiTextButton> Gui::Button(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container,
                                               const std::function<void()>& callback,
                                               bool onNewRow)
    {
        return container->AddElement<GuiTextButton>(onNewRow, text, GuiMetrics::FontSize, callback);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text, GuiWindowID windowID)
    {
        return AddElementToWindow<GuiDropdown>(windowID, text, GuiMetrics::FontSize);
    }

    std::shared_ptr<GuiDropdown> Gui::Dropdown(const std::string& text,
                                               const std::shared_ptr<GuiContainer>& container)
    {
        return container->AddElement<GuiDropdown>(text, GuiMetrics::FontSize);
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
            // We are NOT resizing or moving a window, so we check if we should next loop ...
            HandleHoveringAndPressing(newMousePos);
            // We are not resizing or moving a window, so let's do hover and press stuff
        }
        s_MousePos = newMousePos;
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        if(s_HoveringWindowIndex == -1) { return; }
        if(s_HoveringWindowIndex != 0) {
            // Select window if we are hovering a window and the window is not already selected
            std::rotate(s_Windows.begin(),
                        s_Windows.begin() + s_HoveringWindowIndex, s_Windows.begin() + s_HoveringWindowIndex + 1);
            s_HoveringWindowIndex = 0;
        }
        if(s_HoveringHeader) {
            s_Moving = true;
        }
        if(s_HoveringDiagonalScaling) {
            s_ScalingX = true;
            s_ScalingY = true;
        } else
        {
            if(s_HoverElement != nullptr) {
                s_HoverElement->StartPressing();
            }
        }
    }

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        s_Moving = false;
        s_ScalingX = false;
        s_ScalingY = false;
        if(s_HoverElement != nullptr) {
            s_HoverElement->Release();
        }
    }

    void Gui::OnWindowResize(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<WindowResizeEvent&>(*event);
        GuiMetrics::windowSize = { (float) e.GetNewWidth(), (float) e.GetNewHeight() };
        s_Camera->SetProjection((float) e.GetNewWidth(), (float) e.GetNewHeight());
    }

    void Gui::HandleHoveringAndPressing(const Math::Vec2f& mousePos)
    {
        bool lastIterHoveringDiagonalScaling = s_HoveringDiagonalScaling;
        bool lastIterHoveringHeader = s_HoveringHeader;
        for (int i = 0; i < s_Windows.size(); i++)
        {
            if(!s_Windows[i]->IsHidingElements() && s_Windows[i]->IsHoveringResize(s_MousePos)) {
                if(!lastIterHoveringDiagonalScaling) {
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::NegativeDiagonalResize);
                    s_HoveringDiagonalScaling = true;
                }
                s_HoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringHeader(s_MousePos)) {
                if(!lastIterHoveringHeader) {
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::GrabbingHand);
                    s_HoveringHeader = true;
                }
                s_HoveringWindowIndex = i;
                return;
            }
            if(s_Windows[i]->IsHoveringWindow(mousePos)) {
                if(lastIterHoveringDiagonalScaling || lastIterHoveringHeader) {
                    // We were somewhere else last iteration
                    SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);
                }
                // Update which window we are currently hovering
                s_HoveringWindowIndex = i;
                // Remember which element we hovered last iteration
                std::shared_ptr<GuiElement> lastIterHoverElement = s_HoverElement;
                // Update which element we are hovering right now
                s_HoverElement = s_Windows[i]->GetHoverElement(mousePos);
                if(s_HoverElement != lastIterHoverElement) {
                    if(s_HoverElement != nullptr) {
                        s_HoverElement->StartHovering();
                    }
                    if(lastIterHoverElement != nullptr) {
                        lastIterHoverElement->StopHovering();
                        lastIterHoverElement->StopPressing();
                    }
                }
                // We can only hover 1 window and 1 element so no need to continue.
                s_HoveringDiagonalScaling = false;
                s_HoveringHeader = false;
                return;
            }
        }
        // We are not hovering anything
        s_HoveringDiagonalScaling = false;
        s_HoveringHeader = false;
        s_HoveringWindowIndex = -1;
        if(lastIterHoveringDiagonalScaling || lastIterHoveringHeader) {
            SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);
        }
        if(s_HoverElement != nullptr) {
            // If we did not hit a window, but we are hovering something from last iteration we stop hovering that
            s_HoverElement->StopHovering();
            s_HoverElement->StopPressing();
            s_HoverElement = nullptr;
        }
    }

    bool Gui::MoveOrResizeSelectedWindow(const Math::Vec2f& newMousePos)
    {
        if(s_Windows.empty()) {
            return false;
        }

        Math::Vec2f mouseDelta = newMousePos - s_MousePos;
        Math::Vec2f maxPos = GuiMetrics::windowSize;
        if(s_Moving) {
            s_Windows[0]->Move(mouseDelta, {0.0f, 0.0f}, maxPos);
            return true;
        } else if(s_ScalingX || s_ScalingY)
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