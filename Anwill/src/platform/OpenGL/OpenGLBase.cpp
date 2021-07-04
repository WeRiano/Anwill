#include "OpenGLBase.h"

#include <iostream>

// TODO: Do we need these? Or is there some new opengl feature?
// (See Hazel: "glEnable(DEBUG *something*)
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        std::cout << "File: " << file << std::endl;
        std::cout << "Line: " << line << std::endl;
        std::cout << "Function: " << function << std::endl;
        return false;
    }
    return true;
}