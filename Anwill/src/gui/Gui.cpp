#include "core/Assert.h"
#include "core/Window.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"
#include "gfx/ShaderMacros.h"
#include "events/MouseEvents.h"
#include "events/SystemEvents.h"

namespace Anwill {

    std::unique_ptr<OrthographicCamera> Gui::s_Camera;
    std::vector<GuiWindow> Gui::s_Windows;
    GuiWindowID Gui::s_NextID = 0;
    WindowSettings Gui::s_WindowSettings;
    Math::Vec2f Gui::s_MousePos;
    bool Gui::s_Moving = false;
    bool Gui::s_ScalingX = false;
    bool Gui::s_ScalingY = false;
    bool Gui::s_HoveringDiagonalScaling = false;
    bool Gui::s_HoveringHeader = false;
    int Gui::s_HoveringWindow;
    std::shared_ptr<GuiElement> Gui::s_HoverElement;

    void Gui::Init(const WindowSettings& ws)
    {
        ShaderMacros::SetMacro("AW_GUI_WINDOW_BORDER_SIZE", GuiMetrics::WindowBorderSize);
        ShaderMacros::SetMacro("AW_GUI_WINDOW_HEADER_SIZE", GuiMetrics::WindowHeaderSize);

        s_WindowSettings = ws;
        s_Camera = std::make_unique<OrthographicCamera>((float) ws.width, (float) ws.height);
        GuiElement::s_Font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        GuiElement::s_RectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        GuiWindow::s_WindowShader = Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");
        GuiText::s_Shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiText.glsl");
        GuiButton::s_Shader = Shader::Create("Anwill/res/shaders/OpenGL/GuiButton.glsl");

        SystemEvents::Subscribe<MouseMoveEvent>(OnMouseMove);
        SystemEvents::Subscribe<MouseButtonPressEvent>(OnMousePress);
        SystemEvents::Subscribe<MouseButtonReleaseEvent>(OnMouseRelease);
    }

    void Gui::Render()
    {
        Renderer2D::BeginScene(*s_Camera);
        bool last = false;
        // Render from back to front and highlight the front window as selected
        for(int i = s_Windows.size() - 1; i >= 0; i--) {
            if(i == 0) {
                last = true;
            }
            s_Windows[i].Render(last);
        }
    }

    void Gui::Text(const std::string& text, bool newRow, GuiWindowID windowID)
    {
        newRow ? AddVerticalElement<GuiText>(windowID, text, 13) : AddHorizontalElement<GuiText>(windowID, text, 13);
    }

    void Gui::Button(const std::string& text, const std::function<void()>& callback, bool newRow, GuiWindowID windowID)
    {
        newRow ? AddVerticalElement<GuiButton>(windowID, text, 13, callback) : AddHorizontalElement<GuiButton>(windowID, text, 13, callback);
    }

    GuiWindowID Gui::NewWindow(const std::string& title)
    {
        s_NextID++;

        GuiWindow newWindow(title, s_NextID, {0.0f, 900.0f}, {600.0f, 450.0f});
        s_Windows.push_back(newWindow);

        return s_NextID;
        // TODO: Max nr of windows (id cap or something)
    }

    void Gui::OnMouseMove(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<MouseMoveEvent&>(*event);
        Math::Vec2f newMousePos = {e.GetXPos(), e.GetYPos()};
        if(!UpdateSelectedWindow(newMousePos)) {
            // We are NOT resizing or moving a window, so we check if we should next loop ...
            if (!SetMovingOrResizing()) {
                // We are not resizing or moving a window, so we check if we are hovering any window
                for(unsigned int i = 0; i < s_Windows.size(); i++) {
                    if(s_Windows[i].IsHoveringWindow(newMousePos)) {
                        // Update which window we are currently hovering
                        s_HoveringWindow = i;

                        // Remember which element we hovered last iteration
                        std::shared_ptr<GuiElement> lastIterHoverElement = s_HoverElement;
                        s_HoverElement = s_Windows[i].GetHoverElement(newMousePos);
                        if(s_HoverElement != nullptr && s_HoverElement != lastIterHoverElement) {
                            // We call OnHover() if we are hovering an element and if we weren't already hovering it
                            s_HoverElement->OnHover();
                        } else if(s_HoverElement == nullptr && lastIterHoverElement != nullptr) {
                            // If we stopped hovering an element we need to tell it so
                            lastIterHoverElement->StopHovering();
                        }
                        break;
                    } else {
                        s_HoveringWindow = -1;
                    }
                }
            }
        }
        s_MousePos = newMousePos;
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        if(s_HoveringWindow != -1 && s_HoveringWindow != 0) {
            // Select window if we are hovering a window and the window is not already selected
            std::rotate(s_Windows.begin(), s_Windows.begin() + s_HoveringWindow, s_Windows.begin() + s_HoveringWindow + 1);
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
                s_HoverElement->OnPress();
            }
        }
    }

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        s_Moving = false;
        s_ScalingX = false;
        s_ScalingY = false;
        if(s_HoverElement != nullptr) {
            s_HoverElement->OnRelease();
        }
    }

    bool Gui::SetMovingOrResizing()
    {
        s_HoveringDiagonalScaling = false;
        s_HoveringHeader = false;
        for (unsigned int i = 0; i < s_Windows.size(); i++)
        {
            if (s_Windows[i].IsHoveringResize(s_MousePos))
            {
                s_HoveringDiagonalScaling = true;
                s_HoveringWindow = i;
                SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::NegativeDiagonalResize);
                return true;
            }
            if (s_Windows[i].IsHoveringHeader(s_MousePos))
            {
                s_HoveringHeader = true;
                s_HoveringWindow = i;
                SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::GrabbingHand);
                return true;
            }
        }
        SystemEvents::Add<SetMouseCursorEvent>(SetMouseCursorEvent::CursorType::Arrow);
        return false;
    }

    bool Gui::UpdateSelectedWindow(const Math::Vec2f& newMousePos)
    {
        if(s_Windows.empty()) {
            return false;
        }

        Math::Vec2f mouseDelta = newMousePos - s_MousePos;
        Math::Vec2f maxPos = {(float) s_WindowSettings.width, (float) s_WindowSettings.height};
        if(s_Moving) {
            s_Windows[0].Move(mouseDelta, {0.0f, 0.0f}, maxPos);
            return true;
        } else if(s_ScalingX || s_ScalingY)
        {
            Math::Vec2f windowPos = s_Windows[0].GetPos();
            s_Windows[0].Resize({mouseDelta.GetX(), -mouseDelta.GetY()}, {150.0f, 150.0f}, {maxPos.GetX() - windowPos.GetX(), windowPos.GetY()});
            return true;
        }
        return false;
    }

    GuiWindow& Gui::GetWindow(GuiWindowID id)
    {
        if(id == 0) {
            return s_Windows[0];
        } else {
            for (unsigned int i = 0; i < s_Windows.size(); i++)
            {
                if (s_Windows[i].GetID() == id)
                {
                    return s_Windows[i];
                }
            }
        }
        AW_ERROR("The window you requested does not exist.");
        return s_Windows[0];
    }
}