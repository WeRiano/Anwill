add_library(Gui STATIC
        Gui.cpp GuiElements.cpp
        )

target_include_directories(Gui PUBLIC
        )

target_link_libraries(Gui PUBLIC Core Events)