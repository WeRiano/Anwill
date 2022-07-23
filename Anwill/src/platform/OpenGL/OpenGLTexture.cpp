#include <glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "OpenGLTexture.h"
#include "core/Log.h"

namespace Anwill {

    OpenGLTexture::OpenGLTexture(const std::string& filePath)
    {
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 4);

        if(!data) {
            AW_ERROR("Failed to the following texture: {0}", filePath.c_str());
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture::OpenGLTexture(const unsigned char* bitmapBuffer,
                                 unsigned int bitmapWidth, unsigned int bitmapHeight)
    {
        if(!bitmapBuffer) {
            AW_INFO("The bitmap buffer is empty!");
        }

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED,
                     GL_UNSIGNED_BYTE, bitmapBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void OpenGLTexture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}