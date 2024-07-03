add_subdirectory(containers)
add_subdirectory(elements)
add_subdirectory(styles)

add_library(Gui STATIC
        Gui.cpp GuiLayer.cpp
)
target_link_libraries(Gui PUBLIC Core Events Utils)