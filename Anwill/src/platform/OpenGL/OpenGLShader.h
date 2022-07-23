#pragma once

#include <string>
#include <unordered_map>

#include "gfx/Shader.h"

namespace Anwill {

    class OpenGLShader : public Shader
    {
    public:
        static int s_MaxTextureSlots;

        OpenGLShader(const std::string& filepath);
        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        void SetUniform1i(const int val, const std::string& name) override;
        void SetUniformMat4f(const Math::Mat4f& mat4f, const std::string& name) override;
        void SetUniform1f(const float val, const std::string &name) override;
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
        void SetShaderConstants(std::string& vertSource,
                                std::string& fragSource);
        int GetUniformLocation(const std::string& name);

    };

}