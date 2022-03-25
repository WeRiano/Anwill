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
        virtual void SetUniformMat4f(Mat4f mat, const std::string& name) = 0;
    };

}