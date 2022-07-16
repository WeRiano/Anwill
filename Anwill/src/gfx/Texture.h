#pragma once

#include <memory>
#include <string>

namespace Anwill {

    class Texture
    {
    public:
        static std::shared_ptr<Texture> Create(const std::string& filePath);

        virtual void Bind(unsigned int slot) const = 0;
        virtual void Unbind() const = 0;
    };
}
