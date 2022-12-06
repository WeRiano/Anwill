#include "core/Assert.h"
#include "core/Window.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"

namespace Anwill {

    Mesh GuiElement::s_RectMesh;
    std::unique_ptr<Font> GuiElement::s_Font;

    std::shared_ptr<Shader> GuiWindow::s_WindowShader;

    std::unique_ptr<OrthographicCamera> Gui::s_Camera;
    std::vector<GuiWindow> Gui::s_Windows;
    GuiWindowID Gui::s_NextID = 0;

    void Gui::Init(const WindowSettings& ws)
    {
        Gui::s_Camera = std::make_unique<OrthographicCamera>((float) ws.width,
                                                             (float) ws.height);
        GuiElement::s_Font = std::make_unique<Font>("Sandbox/assets/fonts/arial.ttf");
        GuiElement::s_RectMesh = Mesh::CreateRectMesh(1.0f, 1.0f);
        GuiWindow::s_WindowShader =
                Shader::Create("Anwill/res/shaders/OpenGL/GuiWindow.glsl");
    }

    void Gui::Render()
    {
        Renderer2D::BeginScene(*s_Camera);
        for(unsigned int i = 0; i < s_Windows.size(); i++) {
            s_Windows[i].Render();
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
        newWindow.transform = Math::Mat4f::Translate(newWindow.transform,
                                                     {600.0f, 450.0f, 0.0f});

        s_Windows.push_back(newWindow);

        return s_NextID;
        // TODO: Max nr of windows (id cap)
    }
}