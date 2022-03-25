add_library(Windows STATIC
        WinWindow.cpp
        )

target_include_directories(Windows PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/glfw/include
        )

target_link_directories(Windows PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/glfw/build/windows/src/Debug)

target_link_libraries(Windows PUBLIC Core Events glfw3.lib)