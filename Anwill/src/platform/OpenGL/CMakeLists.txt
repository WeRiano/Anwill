add_library(OpenGL STATIC
        OpenGLContext.cpp OpenGLGraphicsAPI.cpp OpenGLVertexBuffer.cpp OpenGLIndexBuffer.cpp OpenGLVertexArray.cpp
        OpenGLShader.cpp OpenGLTexture.cpp

        ${PROJECT_SOURCE_DIR}/vendor/Glad/src/glad.c
        # We include vendor code here like its src since it is literally just 1 file. Thanks GLAD!
        )

target_include_directories(OpenGL PUBLIC
        ${PROJECT_SOURCE_DIR}/vendor/Glad/include
        ${PROJECT_SOURCE_DIR}/vendor/stb_image
        ${PROJECT_SOURCE_DIR}/vendor/glfw/include
        )

target_link_libraries(OpenGL PUBLIC Core opengl32 glfw ${GLFW_LIBRARIES})