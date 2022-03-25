add_library(Graphics STATIC
        Renderer.cpp GraphicsContext.cpp GraphicsAPI.cpp VertexBuffer.cpp IndexBuffer.cpp
        VertexArray.cpp Shader.cpp

        )

target_link_libraries(Graphics PUBLIC OpenGL)