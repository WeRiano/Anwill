add_library(Graphics STATIC
        Renderer.cpp GraphicsContext.cpp GraphicsAPI.cpp VertexBuffer.cpp IndexBuffer.cpp
        VertexArray.cpp Shader.cpp Camera.cpp Mesh.cpp

        )

target_link_libraries(Graphics PUBLIC OpenGL Math)