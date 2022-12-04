add_library(Gui STATIC
        Gui.cpp

        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/imgui_demo.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui/backends/imgui_impl_opengl3.cpp
        )

target_include_directories(Gui PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/imgui
        )

target_link_libraries(Gui PUBLIC Core Events)