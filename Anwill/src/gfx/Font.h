#pragma once

#include <string>
#include <map>

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Anwill {

    struct Glyph {
        std::shared_ptr<Texture> texture;
        int width, height;
        int bearingX, bearingY;
        int advance;
        float x0, x1, y0, y1;
    };

    class Font
    {
    public:
        Font(const std::string& filePath);

        /**
         * @brief Prepare the necessary graphical tools to render some text in @this font
         * @param text the text to be rendered
         * @param shader the shader that should be used when rendering the text
         * @return The final advance x coordinate where additional text *should* continue
         */
        int Prepare(const std::string& text, const std::shared_ptr<Shader>& shader,
                      int startXPos);
        void Done();

    private:
        std::shared_ptr<VertexBuffer> m_VB;
        std::shared_ptr<VertexArray> m_VA;
        std::map<unsigned char, Glyph> m_Characters;
        unsigned int m_ID;
    };
}