add_library(Graphics STATIC Renderer.cpp GraphicsContext.cpp ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/src/glad.c)
target_include_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/include
        ${PROJECT_SOURCE_DIR}/Anwill/src)
target_link_libraries(Graphics PRIVATE)