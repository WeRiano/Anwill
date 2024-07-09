#include <imgui.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "gfx/Renderer.h"

#include "ImguiLayer.h"

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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        if(ImGui::Begin("Anwill sandbox selector")) {
            ImGui::Text("Hello world!");
        }

        ImGui::End();

        //ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}