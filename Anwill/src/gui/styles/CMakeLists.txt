add_library(GuiStyles STATIC
        GuiStyling.cpp
)
#target_include_directories(GuiStyles PUBLIC Core Events Utils)
target_link_libraries(GuiStyles PUBLIC Core)