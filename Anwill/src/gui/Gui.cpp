#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "core/Assert.h"
#include "gui/Gui.h"
#include "gfx/Renderer.h"

namespace Anwill {

    void Gui::Init(void* nativeWindow)
    {
        AW_ASSERT(Renderer::GetAPI() == GraphicsAPI::API::OpenGL,
                  "GUI does not support this graphics platform!");
        #ifdef AW_PLATFORM_WINDOWS
        auto glfwWindow = static_cast<GLFWwindow*>(nativeWindow);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, false);
        ImGui_ImplOpenGL3_Init();
        #else
            #error GUI does not support this platform!
        #endif
    }

    void Gui::Begin() {
        AW_ASSERT(Renderer::GetAPI() == GraphicsAPI::API::OpenGL,
                  "GUI does not support this graphics platform!");
        ImGui_ImplOpenGL3_NewFrame();
        #ifdef AW_PLATFORM_WINDOWS
        ImGui_ImplGlfw_NewFrame();
        #endif
        ImGui::NewFrame();
    }

    void Gui::Render()
    {
        AW_ASSERT(Renderer::GetAPI() == GraphicsAPI::API::OpenGL,
                  "GUI does not support this graphics platform!");
        ImGui::ShowDemoWindow(); // TODO: REMOVE! (Temp)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Gui::Terminate()
    {
        AW_ASSERT(Renderer::GetAPI() == GraphicsAPI::API::OpenGL,
                  "GUI does not support this graphics platform!");
        ImGui_ImplOpenGL3_Shutdown();
        #ifdef AW_PLATFORM_WINDOWS
        ImGui_ImplGlfw_Shutdown();
        #endif
        ImGui::DestroyContext();
    }
}