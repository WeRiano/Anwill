#include <glad.h>

#include "gfx/ShaderMacros.h"
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
            AW_ERROR("Failed to parse vertex and fragment shader correctly for {0}.",
                     filepath);
            return;
        }
        /* TODO: We assume vertex shader comes before fragment shader now. Fix? (Generalize?)
                  Probably not */

        ShaderMacros::ReplaceAllIdentifiers(shaderSrc[0]);
        ShaderMacros::ReplaceAllIdentifiers(shaderSrc[1]);


        m_ID = glCreateProgram();
        unsigned int vertID = CompileShader(GL_VERTEX_SHADER, shaderSrc[0]);
        unsigned int fragID = CompileShader(GL_FRAGMENT_SHADER, shaderSrc[1]);
        AttachLinkAndValidateShader(vertID, fragID);
    }

    OpenGLShader::~OpenGLShader()
    {
        //glDeleteProgram(m_ID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_ID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetUniform1i(const int val, const std::string& name)
    {
        int location = GetUniformLocation(name);
        glUniform1iv(location, 1, &val);
    }

    void OpenGLShader::SetUniformMat4f(const Math::Mat4f& mat4f, const std::string& name)
    {
        //Bind();
        int location = GetUniformLocation(name);
        // TODO: Count is always 1 for now, generalize when we actually need a vector of mat4fs
        glUniformMatrix4fv(location, 1, GL_FALSE, mat4f.GetInternal());
        //Unbind();
    }

    void OpenGLShader::SetUniform1f(const float val, const std::string &name)
    {
        int location = GetUniformLocation(name);
        glUniform1fv(location, 1, &val);
    }

    void OpenGLShader::SetUniformVec2f(const Math::Vec2f& vec2f, const std::string &name)
    {
        float temp[2] = {
                vec2f.GetX(), vec2f.GetY()
        };
        int location = GetUniformLocation(name);
        glUniform2fv(location, 1, temp);
    }

    void OpenGLShader::SetUniformVec3f(const Math::Vec3f &vec3f, const std::string &name)
    {
        float temp[3] = {
                vec3f.GetX(), vec3f.GetY(), vec3f.GetZ()
        };
        int location = GetUniformLocation(name);
        glUniform3fv(location, 1, temp);
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

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        int location;
            if (m_LocationCache.contains(name))
        {
            location = m_LocationCache[name];
        } else
        {
            location = glGetUniformLocation(m_ID, name.c_str());
            if (location == -1)
            {
                AW_ERROR("OpenGL failed to retrieve the location of the following "
                         "uniform: {0}", name);
            } else
            {
                m_LocationCache[name] = location;
            }
        }
        return location;
    }
}