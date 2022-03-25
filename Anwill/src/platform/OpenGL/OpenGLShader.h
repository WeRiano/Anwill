#pragma once

#include <string>
#include <unordered_map>

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

        void Bind() const override;
        void Unbind() const override;

        // TODO: UNIFORM LOCATION CACHING!
        void SetUniformMat4f(Mat4f mat, const std::string& name) override;

    private:
        unsigned int m_ID;
        std::unordered_map<std::string, int> m_LocationCache;

        /**
         * Compile individual subshader (fragment or vertex)
         */
        unsigned int CompileShader(unsigned int glShaderType, const std::string& shaderSrc);
        void AttachLinkAndValidateShader(unsigned int vertexShaderID,
                                         unsigned int fragmentShaderID);
        int GetUniformLocation(const std::string& name);

    };

}