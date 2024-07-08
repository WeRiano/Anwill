add_library(ImGui STATIC ImguiLayer.cpp)

target_include_directories(ImGui PUBLIC
        ${PROJECT_SOURCE_DIR}/vendor/imgui
)