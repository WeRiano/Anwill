add_library(GuiContainers STATIC
        GuiContainer.cpp
        GuiWindow.cpp
)
target_include_directories(GuiContainers PUBLIC Core Events Utils GuiElements GuiStyles)
target_link_libraries(GuiContainers PUBLIC Core Events Utils)