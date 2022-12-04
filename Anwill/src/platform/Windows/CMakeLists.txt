add_library(Windows STATIC
        WinWindow.cpp WinInput.cpp
        )


target_include_directories(Windows PUBLIC ${PROJECT_SOURCE_DIR}/Anwill/vendor/glfw/include)

target_link_libraries(Windows PUBLIC Core Events glfw ${GLFW_LIBRARIES})