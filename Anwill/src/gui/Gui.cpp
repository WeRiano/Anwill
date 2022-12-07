#include "core/Assert.h"
#include "core/Window.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"
#include "events/MouseEvents.h"
#include "events/SystemEvents.h"
#include "math/Algo.h"

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
        // TODO: Max nr of windows (id cap)
    }

    void Gui::OnMouseMove(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<MouseMoveEvent&>(*event);
        auto& mat = s_Windows[0].transform;
        Math::Vec3f oldMousePos = {s_MousePos.GetX(), s_MousePos.GetY(), 0.0f};
        Math::Vec3f newMousePos = {e.GetXPos(), e.GetYPos(), 0.0f};
        Math::Vec3f delta = newMousePos - oldMousePos;
        if(s_Moving) {
            mat = Math::Mat4f::Translate(mat, delta);
        }
        if(s_ScalingX) {
            float oldSize = mat.GetScale().GetX();
            float newSize = (newMousePos.GetX() - (mat.GetTranslateVector().GetX() - oldSize / 2.0f));
            float scaleFactor = newSize/oldSize;
            mat = Math::Mat4f::Scale(mat, {scaleFactor, 1.0f, 1.0f});
            mat = Math::Mat4f::Translate(mat, {(newSize - oldSize) * 0.5f, 0.0f, 0.0f});
            // TODO: Minimum size
        }
        if(s_ScalingY) {
            float oldSize = mat.GetScale().GetY();
            float newSize = (mat.GetTranslateVector().GetY() + oldSize / 2.0f) - newMousePos.GetY();
            float scaleFactor = newSize/oldSize;
            mat = Math::Mat4f::Scale(mat, {1.0f, scaleFactor, 1.0f});
            mat = Math::Mat4f::Translate(mat, {0.0f, -(newSize - oldSize) * 0.5f, 0.0f});
            // TODO: Minimum size
        }
        s_MousePos = {e.GetXPos(), e.GetYPos()};
    }

    void Gui::OnMousePress(std::unique_ptr<Event>& event)
    {
        // TODO: Helper functions to cleanup

        // TODO: Don't hard code (use shader macro system!)
        float borderSize = 8.0f;
        float headerSize = borderSize * 2.5f;
        for(unsigned int i = 0; i < s_Windows.size(); i++) {
            auto origin = s_Windows[i].transform.GetTranslateVector();
            auto size = s_Windows[i].transform.GetScale();
            if(i == 0 && Math::Algo::IsPointInsideCircle({origin.GetX() + size.GetX() / 2,
                                                          origin.GetY() - size.GetY() / 2},
                                                         10.0f,
                                                         s_MousePos)) {
                AW_INFO("IS INSIDE!");
                s_ScalingX = true;
                s_ScalingY = true;
            }
            if(Math::Algo::IsPointInsideRectangle({origin.GetX() - size.GetX() / 2,
                                                   origin.GetY() + size.GetY() / 2},
                                                  {origin.GetX() + size.GetX() / 2,
                                                   origin.GetY() + size.GetY() / 2},
                                                  {origin.GetX() + size.GetX() / 2,
                                                   origin.GetY() - size.GetY() / 2},
                                                  {origin.GetX() - size.GetX() / 2,
                                                   origin.GetY() - size.GetY() / 2},
                                                  s_MousePos)) {
                if(i > 0) {
                    // Move this window to the front since it has been selected
                    std::rotate(s_Windows.begin(),
                                s_Windows.begin() + i,
                                s_Windows.begin() + i + 1);
                }
                if(Math::Algo::IsPointInsideRectangle({origin.GetX() - size.GetX() / 2,
                                                       origin.GetY() + size.GetY() / 2},
                                                      {origin.GetX() + size.GetX() / 2,
                                                       origin.GetY() + size.GetY() / 2},
                                                      {origin.GetX() + size.GetX() / 2,
                                                       origin.GetY() + size.GetY() / 2
                                                       - headerSize},
                                                      {origin.GetX() - size.GetX() / 2,
                                                       origin.GetY() + size.GetY() / 2
                                                       - headerSize},
                                                      s_MousePos)) {
                    s_Moving = true;
                }
                break;
            }
        }
    }

    void Gui::OnMouseRelease(std::unique_ptr<Event>& event)
    {
        s_Moving = false;
        s_ScalingX = false;
        s_ScalingY = false;
    }
}