#pragma once

#include <memory>
#include <string>

namespace Anwill {

    class Shader
    {
    public:
        static std::shared_ptr<Shader> Create(const std::string& filepath);
    };

}