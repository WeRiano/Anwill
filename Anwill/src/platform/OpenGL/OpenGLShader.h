#pragma once

#include <string>

#include "gfx/Shader.h"

namespace Anwill {

    struct ShaderSrc
    {
        const std::string vs, fs;
    };

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);

    private:
        unsigned int m_ID;

        /**
         * Compile individual subshader (fragment or vertex)
         */
        unsigned int CompileShader(unsigned int glShaderType, const std::string& shaderSrc);
        void AttachLinkAndValidateShader(unsigned int vertexShaderID,
                                         unsigned int fragmentShaderID);

    };

}