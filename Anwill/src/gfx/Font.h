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
        Math::Vec2f Prepare(const std::string& text, const std::shared_ptr<Shader>& shader,
                      const Math::Vec2f& startXPos);

        /**
         * @brief Get the distance of some text in the positive y direction, positive
         *        x direction and in the negative y direction.
         */
        std::string GetLargestSubstr(const std::string& text, unsigned int maxWidth);
        unsigned int GetStringWidth(const std::string& text);
        void Done();

        static float GetScaleValue(unsigned int fontSize);

    private:
        std::shared_ptr<VertexBuffer> m_VB;
        std::shared_ptr<VertexArray> m_VA;
        std::map<unsigned char, Glyph> m_Characters;
        unsigned int m_ID;
        float m_NewlineHeight;

        unsigned int GetTextureSlot(std::map<unsigned char, unsigned int> map,
                                    unsigned int& textureSlotCounter,
                                    const std::shared_ptr<Shader>& shader,
                                    unsigned char c);
        void SetCharVertices(float* vertices, Math::Vec2f& advance, const Glyph& glyph, unsigned int offset,
                             unsigned int textureSlot);
        void SetEscapeCharVertices(float* vertices, Math::Vec2f& advance, unsigned char escapeChar,
                                   unsigned int offset, unsigned int textureSlot);
    };
}
