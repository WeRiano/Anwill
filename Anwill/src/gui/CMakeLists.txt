add_subdirectory(containers)
add_subdirectory(elements)
add_subdirectory(styles)

add_library(Gui STATIC
        Gui.cpp GuiLayer.cpp

        #elements/GuiButton.cpp elements/GuiCheckbox.cpp elements/GuiDropdown.cpp elements/GuiElement.cpp
        #elements/GuiIcon.cpp elements/GuiImage.cpp elements/GuiInputText.cpp elements/GuiRadioButton.cpp
        #elements/GuiText.cpp elements/GuiTextButton.cpp elements/GuiTooltip.cpp

        #containers/GuiContainer.cpp
        #containers/GuiWindow.cpp

        #styles/GuiStyling.cpp
)
target_link_libraries(Gui PUBLIC GuiContainers GuiElements GuiStyles)