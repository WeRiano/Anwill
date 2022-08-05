#pragma once

#include <memory>
#include <string>

namespace Anwill {

    class Texture
    {
    public:
        static std::shared_ptr<Texture> Create(const std::string& filePath);
        static std::shared_ptr<Texture> Create(const unsigned char* bitmapBuffer,
                                               unsigned int bitmapWidth,
                                               unsigned int bitmapHeight);

        virtual void Bind(unsigned int slot = 0) const = 0;
        virtual void Unbind() const = 0;
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

    protected:
        int m_Width, m_Height;
    };
}
