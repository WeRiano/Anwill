#include <GLFW/glfw3.h>

#include "ImguiLayer.h"
#include "gfx/Renderer.h"

namespace Anwill {

    void ImguiLayer::Init(const Window& window)
    {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
        io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window.GetNativeWindow(), true);
        if(Renderer::GetAPI() == GraphicsAPI::API::OpenGL)
        {
            ImGui_ImplOpenGL3_Init("#version 460");
        }

        ImGui::StyleColorsLight();
    }

    ImguiLayer::ImguiLayer(const unsigned int ups, const Window& window)
        : Layer(ups)
    {
        Init(window);
    }

    void ImguiLayer::Update(const Timestamp& timestamp)
    {
        Layer::Update(timestamp);

        //ImGui::ShowDemoWindow();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}