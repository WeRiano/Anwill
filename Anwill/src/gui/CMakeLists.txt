add_library(Gui STATIC
            Gui.cpp GuiLayer.cpp
        )

target_include_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/
        )

target_link_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/
        )

target_link_libraries(Graphics PUBLIC Events Core)