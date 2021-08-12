#include <glad.h>

#include "platform/OpenGL/OpenGLShader.h"
#include "utils/FileReader.h"
#include "core/Assert.h"
#include "core/Log.h"

namespace Anwill {

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        auto shaderSrc = Anwill::FileIO::LoadFileToStrVec(filepath,
                                                          {"#shadertype vertex", "#shadertype fragment"});
        if (shaderSrc.size() != 2)
        {
            AW_ERROR("Failed to load vertex and fragment shader correctly for {0}.", filepath);
            AW_ASSERT(false);
        }
        /* TODO: We assume vertex shader comes before fragment shader now. Fix? (Generalize?)
                  Probably not */

        m_ID = glCreateProgram();
        unsigned int vertID = CompileShader(GL_VERTEX_SHADER, shaderSrc[0]);
        unsigned int fragID = CompileShader(GL_FRAGMENT_SHADER, shaderSrc[1]);
        AttachLinkAndValidateShader(vertID, fragID);
    }

    unsigned int OpenGLShader::CompileShader(unsigned int glShaderType, const std::string& shaderSrc)
    {
        unsigned int id = glCreateShader(glShaderType);
        const char* srcArr[] = { shaderSrc.c_str() };
        glShaderSource(id, 1, srcArr, nullptr);
        glCompileShader(id);

        // Error handling
        int compileStatus;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            AW_ERROR("Failed to compile {0} shader! {1}",
                     (glShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"),
                     message);
            glDeleteShader(id);
            AW_ASSERT(false);
        }

        return id;
    }

    void OpenGLShader::AttachLinkAndValidateShader(unsigned int vertexShaderID,
                                                   unsigned int fragmentShaderID)
    {
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        int linkStatus;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &linkStatus);
        if(linkStatus == GL_FALSE)
        {
            int length;
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetProgramInfoLog(m_ID, length, &length, message);
            AW_ERROR("Failed to link shader program! {0}", message);
            glDeleteProgram(m_ID);
        }

        // Checks to see whether the executables contained in program can
        // execute given the current OpenGL state
        glValidateProgram(m_ID);

        // Delete the individual shaders after we have linked them to the program
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }
}