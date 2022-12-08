#include "core/Assert.h"
#include "core/Window.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"
#include "events/MouseEvents.h"
#include "events/SystemEvents.h"

namespace Anwill {

    Mesh GuiElement::s_RectMesh;
    std::unique_ptr<Font> GuiElement::s_Font;

    std::shared_ptr<Shader> GuiWindow::s_WindowShader;

    std::unique_ptr<OrthographicCamera> Gui::s_Camera;
    std::vector<GuiWindow> Gui::s_Windows;
    GuiWindowID Gui::s_NextID = 0;
    Math::Vec2f Gui::s_MousePos;
    bool Gui::s_Moving = false;
    bool Gui::s_ScalingX = false;
    bool Gui::s_ScalingY = false;
    bool Gui::s_HoveringDiagonalScaling = false;
    bool Gui::s_HoveringHeader = false;
    unsigned int Gui::s_HoveringWindow;

    void Gui::Init(const WindowSettings& ws)
    {
        Gui::s_Camera = std::make_unique<OrthographicCamera>((float) ws.width,
                                                             (float) ws.height);
        GuiElement::s_Font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        GuiElement::s_RectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        GuiWindow::s_WindowShader =
                Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");

        SystemEvents::Subscribe<MouseMoveEvent>(OnMouseMove);
        SystemEvents::Subscribe<MouseButtonPressEvent>(OnMousePress);
        SystemEvents::Subscribe<MouseButtonReleaseEvent>(OnMouseRelease);
    }

    void Gui::Render()
    {
        Renderer2D::BeginScene(*s_Camera);
        bool last = false;
        for(int i = s_Windows.size() - 1; i >= 0; i--) {
            if(i == 0) {
                last = true;
            }
            s_Windows[i].Render(last);
        }
    }

    GuiWindowID Gui::NewWindow(const std::string& title)
    {
        s_NextID++;

        GuiWindow newWindow;
        newWindow.id = s_NextID;
        newWindow.title = title;
        newWindow.transform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                 {200.0f, 300.0f, 0.0f});
        if(title != "Test") {
            newWindow.transform = Math::Mat4f::Translate(newWindow.transform,
                                                         {600.0f, 450.0f, 0.0f});
        } else
        {
            newWindow.transform = Math::Mat4f::Translate(newWindow.transform,
                                                         {200.0f, 450.0f, 0.0f});
        }

        s_Windows.push_back(newWindow);

        return s_NextID;
        // TODO: Max nr of windows (id cap or something)
    }

    void Gui::OnMouseMove(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<MouseMoveEvent&>(*event);
        Math::Vec2f newMousePos = {e.GetXPos(), e.GetYPos()};
        if(!UpdateSelectedWindow(newMousePos)) {
            CheckForHovering(newMousePos);
            SetMouseCursor(newMousePos);
        }
        s_MousePos = {e.GetXPos(), e.GetYPos()};
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        if(s_HoveringHeader) {
            std::rotate(s_Windows.begin(), s_Windows.begin() + s_HoveringWindow, s_Windows.begin() + s_HoveringWindow + 1);
            s_Moving = true;
        }
        if(s_HoveringDiagonalScaling) {
            std::rotate(s_Windows.begin(), s_Windows.begin() + s_HoveringWindow, s_Windows.begin() + s_HoveringWindow + 1);
            s_ScalingX = true;
            s_ScalingY = true;
        } else
        {
            for (unsigned int i = 0; i < s_Windows.size(); i++)
            {
                if (s_Windows[i].IsHoveringWindow(s_MousePos))
                {
                    std::rotate(s_Windows.begin(), s_Windows.begin() + i, s_Windows.begin() + i + 1);
                    break;
                }
            }
        }
    }

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        s_Moving = false;
        s_ScalingX = false;
        s_ScalingY = false;
    }

    void Gui::SetMouseCursor(const Math::Vec2f& newMousePos)
    {
        if (s_HoveringDiagonalScaling)
        {
            SystemEvents::Add<SetMouseCursorEvent>(
                    SetMouseCursorEvent::CursorType::NegativeDiagonalResize);
        } else if (s_HoveringHeader)
        {
            SystemEvents::Add<SetMouseCursorEvent>(
                    SetMouseCursorEvent::CursorType::GrabbingHand);
        } else
        {
            SystemEvents::Add<SetMouseCursorEvent>(
                    SetMouseCursorEvent::CursorType::Arrow);
        }
    }

    void Gui::CheckForHovering(const Math::Vec2f& newMousePos)
    {
        s_HoveringDiagonalScaling = false;
        s_HoveringHeader = false;
        for (unsigned int i = 0; i < s_Windows.size(); i++)
        {
            if (s_Windows[i].IsHoveringResize(s_MousePos))
            {
                s_HoveringDiagonalScaling = true;
                s_HoveringWindow = i;
                break;
            }
            if (s_Windows[i].IsHoveringHeader(s_MousePos))
            {
                s_HoveringHeader = true;
                s_HoveringWindow = i;
                break;
            }
        }
    }

    bool Gui::UpdateSelectedWindow(const Math::Vec2f& newMousePos)
    {
        auto& mat = s_Windows[0].transform;
        Math::Vec3f oldMousePos = {s_MousePos.GetX(), s_MousePos.GetY(), 0.0f};
        Math::Vec3f newMousePos3f = {newMousePos.GetX(), newMousePos.GetY(), 0.0f};
        Math::Vec3f mouseDelta = newMousePos3f - oldMousePos;
        if(s_Moving) {
            mat = Math::Mat4f::Translate(mat, mouseDelta);
            return true;
        } else if(s_ScalingX || s_ScalingY)
        {
            if (s_ScalingX)
            {
                float oldWidth = mat.GetScale().GetX();
                float newWidth = oldWidth + mouseDelta.GetX();
                float scaleFactor = newWidth / oldWidth;
                mat = Math::Mat4f::Scale(mat, {scaleFactor, 1.0f, 1.0f});
                mat = Math::Mat4f::Translate(mat, {mouseDelta.GetX() / 2.0f, 0.0f, 0.0f});
            }
            if (s_ScalingY)
            {
                float oldHeight = mat.GetScale().GetY();
                float newHeight = oldHeight - mouseDelta.GetY();
                float scaleFactor = newHeight / oldHeight;
                mat = Math::Mat4f::Scale(mat, {1.0f, scaleFactor, 1.0f});
                mat = Math::Mat4f::Translate(mat, {0.0f, mouseDelta.GetY() / 2.0f, 0.0f});
            }
            return true;
        }
        return false;
    }
}