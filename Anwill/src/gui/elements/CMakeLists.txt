add_library(GuiElements STATIC
        GuiButton.cpp
        GuiCheckbox.cpp
        GuiDropdown.cpp
        GuiElement.cpp
        GuiIcon.cpp
        GuiImage.cpp
        GuiInputText.cpp
        GuiRadioButton.cpp
        GuiText.cpp
        GuiTextButton.cpp
        GuiTooltip.cpp
)
target_include_directories(GuiElements PUBLIC Core Events Utils Containers GuiStyle)
target_link_libraries(GuiElements PUBLIC Core Events Utils)