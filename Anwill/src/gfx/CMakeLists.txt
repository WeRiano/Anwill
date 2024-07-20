add_library(Graphics STATIC
        Renderer.cpp GraphicsContext.cpp GraphicsAPI.cpp VertexBuffer.cpp IndexBuffer.cpp
        VertexArray.cpp Shader.cpp Camera.cpp Mesh.cpp Texture.cpp Font.cpp SpriteSheet.cpp
        Renderer2D.cpp SpriteAnimation.cpp Sprite.cpp CameraController.cpp
)

target_include_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/vendor/freetype/include
        ${PROJECT_SOURCE_DIR}/vendor/stb_truetype
        )

target_link_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/vendor/freetype/lib
        )

target_link_libraries(Graphics PUBLIC OpenGL Math freetyped.lib)