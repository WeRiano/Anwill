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

        void SetUniformMat4f(const Math::Mat4f& mat4f, const std::string& name) override;
        void SetUniformVec1f(const float val, const std::string &name) override;
        void SetUniformVec2f(const Math::Vec2f& vec2f, const std::string &name) override;
        void SetUniformVec3f(const Math::Vec3f& vec3f, const std::string &name) override;

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