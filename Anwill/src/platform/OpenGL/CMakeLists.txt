add_library(OpenGL STATIC OpenGLContext.cpp OpenGLGraphicsAPI.cpp
        OpenGLVertexBuffer.cpp OpenGLIndexBuffer.cpp OpenGLVertexArray.cpp
        OpenGLShader.cpp
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/src/glad.c)
target_include_directories(OpenGL PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/include
        ${PROJECT_SOURCE_DIR}/Anwill/src
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Willog/include
        )
target_link_libraries(OpenGL PUBLIC Core opengl32)