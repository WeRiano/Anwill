#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Imgui.h"
#include "gfx/Renderer.h"


namespace Anwill {

    void Imgui::Init(const Window& window)
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

    void Imgui::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Imgui::EndFrame()
    {
        ImGui::EndFrame();
    }

    void Imgui::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}