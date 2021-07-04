add_library(OpenGL STATIC OpenGLContext.cpp OpenGLBase.cpp ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/src/glad.c)
target_include_directories(OpenGL PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Glad/include
        ${PROJECT_SOURCE_DIR}/Anwill/src
        )
target_link_libraries(OpenGL PUBLIC Core opengl32)