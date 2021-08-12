add_library(Graphics STATIC
        Renderer.cpp GraphicsContext.cpp GraphicsAPI.cpp VertexBuffer.cpp IndexBuffer.cpp
        VertexArray.cpp Shader.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/src/glad.c)
target_include_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/include
        ${PROJECT_SOURCE_DIR}/Anwill/src
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Willog/include)
target_link_libraries(Graphics PRIVATE)