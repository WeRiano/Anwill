#pragma once

#include <memory>
#include <string>

#include "math/Mat4f.h"

namespace Anwill {

    class Shader
    {
    public:
        static std::shared_ptr<Shader> Create(const std::string& filepath);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // Uniforms
        virtual void SetUniform1i(const int val, const std::string& name) = 0;
        virtual void SetUniformMat4f(const Math::Mat4f& mat4f, const std::string& name) = 0;
        virtual void SetUniform1f(const float val, const std::string& name) = 0;
        virtual void SetUniformVec2f(const Math::Vec2f& vec2f, const std::string& name) = 0;
        virtual void SetUniformVec3f(const Math::Vec3f& vec3f, const std::string& name) = 0;

    };

}