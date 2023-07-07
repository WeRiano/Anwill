#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"
#include "core/Log.h"

namespace Anwill {

    Font::Font(const std::string& filePath)
    {
        FT_Library ftLib;
        FT_Face ftFace;
        FT_Error error = FT_Init_FreeType(&ftLib);
        if (error)
        {
            AW_ERROR("FreeType library failed to initialize!");
            return;
        }
        error = FT_New_Face(ftLib,
                            filePath.c_str(),
                            0,
                            &ftFace);

        if (error == FT_Err_Unknown_File_Format)
        {
            AW_ERROR("FreeType: The font file could be opened and read, "
                     "but it appears that its font format is unsupported: {0}",
                     filePath.c_str());
            return;
        }
        else if (error)
        {
            AW_ERROR("FreeType: The font file could not "
                     "be opened or read, or that it is broken: {0}",
                     filePath.c_str());
            return;
        }

        FT_Set_Pixel_Sizes(ftFace, 0, 42);

        //m_NewlineHeight = ftFace->height;
        m_NewlineHeight = ftFace->height / 64;

        for(unsigned char c = 0; c < 128; c++)
        {
            error = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
            if(error)
            {
                AW_ERROR("TrueType: Error when loading glyph from the following font "
                         "face: {0}", filePath.c_str());
                return;
            }

            Glyph newGlyph;
            newGlyph.width = ftFace->glyph->bitmap.width;
            newGlyph.height = ftFace->glyph->bitmap.rows;
            newGlyph.bearingX = ftFace->glyph->bitmap_left;
            newGlyph.bearingY = ftFace->glyph->bitmap_top;
            newGlyph.advance = ftFace->glyph->advance.x;

            // Just skip calculating this every time we render, do it once here instead
            newGlyph.x0 = newGlyph.bearingX;
            newGlyph.y0 = newGlyph.bearingY - newGlyph.height;
            newGlyph.x1 = newGlyph.x0 + newGlyph.width;
            newGlyph.y1 = newGlyph.y0 + newGlyph.height;

            newGlyph.texture = Anwill::Texture::Create(ftFace->glyph->bitmap.buffer,
                                                       ftFace->glyph->bitmap.width,
                                                       ftFace->glyph->bitmap.rows);
            m_Characters.insert(std::pair<unsigned char, Glyph>(c, newGlyph));
        }
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLib);

        m_VA = VertexArray::Create();
        m_VB = VertexBuffer::Create(sizeof(float) * 20000); // TODO: Max text size
        auto layout = BufferLayout({
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float)
        });

        m_VA->AddBuffer(*m_VB, layout);
    }

    Math::Vec2f Font::Prepare(const std::string& text, const std::shared_ptr<Shader>& shader,
                      const Math::Vec2f& startPos)
    {
        // Vertex setup

        // One glyph is 6 vertices
        // One vertex is 2 coordinates, 2 tex coordinates, 1 bitmap index
        // One glyph per text character
        unsigned int glyphSize = 6 * 5;
        auto vertices = new float[text.size() * glyphSize];
        Math::Vec2f advance = startPos;

        // Texture slot setup
        std::map<unsigned char, unsigned int> textureSlotMap;
        unsigned int newTextureSlotCounter = 0;

        // Flag that keeps track if we've encountered an escape character
        bool isEscaping = false;

        // Loop through each character in the text
        for(unsigned int i = 0; i < text.size(); i++)
        {
            unsigned char c = text[i];
            unsigned int thisIterationTextSlot = GetTextureSlot(textureSlotMap, newTextureSlotCounter,
                                                                shader, c);
            unsigned int offset = i * glyphSize;
            Glyph g = m_Characters[c];

            if(isEscaping) {
                // Last iter we encountered a backslash
                // Look at what escape char we got and act accordingly
                SetEscapeCharVertices(vertices, advance, c, offset, thisIterationTextSlot);
                isEscaping = false;
                continue;
            }
            if(c == '\\') {
                // We encountered a backslash this iter. Escaping next iter.
                isEscaping = true;
                continue;
            }

            SetCharVertices(vertices, advance, g, offset, thisIterationTextSlot);
        }

        // Update the GPU with new vertex array
        m_VB->DynamicUpdate(vertices, sizeof(float) * text.size() * glyphSize);
        m_VA->Bind();

        // Delete the temporary one
        delete[] vertices;
        return advance;
    }

    std::string Font::GetLargestSubstr(const std::string& text, unsigned int maxWidth)
    {
        unsigned int curWidth = 0;
        std::string result;
        for(unsigned int i = 0; i < text.size(); i++)
        {
            char c = text[i];
            Glyph g = m_Characters[c];
            unsigned int widthIncrease = (g.advance >> 6);
            if(curWidth + widthIncrease > maxWidth)
            {
                return result;
            }
            curWidth += widthIncrease;
            result += c;
        }
        return result;
    }

    unsigned int Font::GetStringWidth(const std::string& text)
    {
        unsigned int curWidth = 0;
        for(unsigned int i = 0; i < text.size(); i++)
        {
            char c = text[i];
            Glyph g = m_Characters[c];
            curWidth += (g.advance >> 6);
        }
        return curWidth;
    }

    void Font::Done()
    {
        m_VA->Unbind();
    }

    float Font::GetScaleValue(unsigned int fontSize)
    {
        return (float) fontSize / 42.0f;
    }

    unsigned int Font::GetTextureSlot(std::map<unsigned char, unsigned int> map,
                              unsigned int& textureSlotCounter,
                              const std::shared_ptr<Shader>& shader,
                              unsigned char c)
    {
        // Getting the correct texture slot. If we have encountered the character previously, it will
        // be stored in textureSlotMap. Otherwise, we create a new one.

        shader->Bind();
        if (map.contains(c)) {
            // This character has already been used, lets find out which texture slot it uses
            return map[c];
        } else {
            // We create a new texture slot and use it (feed it to the shader).
            unsigned int result = textureSlotCounter;
            map[c] = result;
            textureSlotCounter++;

            m_Characters[c].texture->Bind(result);
            shader->SetUniform1i(result, "u_TextBitmaps[" + std::to_string(result) + "]");
            return result;
        }
    }

    void Font::SetCharVertices(float* vertices, Math::Vec2f& advance, const Glyph& glyph,
                               unsigned int offset, unsigned int textureSlot)
    {

        float x0 = glyph.x0 + advance.GetX();
        float x1 = glyph.x1 + advance.GetX();
        float y0 = glyph.y0 - advance.GetY();
        float y1 = glyph.y1 - advance.GetY();

        vertices[offset + 0] = x0;
        vertices[offset + 1] = y1;
        vertices[offset + 2] = 0.0f;
        vertices[offset + 3] = 0.0f;
        vertices[offset + 4] = (float) textureSlot;

        vertices[offset + 5] = x0;
        vertices[offset + 6] = y0;
        vertices[offset + 7] = 0.0f;
        vertices[offset + 8] = 1.0f;
        vertices[offset + 9] = (float) textureSlot;

        vertices[offset + 10] = x1;
        vertices[offset + 11] = y0;
        vertices[offset + 12] = 1.0f;
        vertices[offset + 13] = 1.0f;
        vertices[offset + 14] = (float) textureSlot;

        vertices[offset + 15] = x0;
        vertices[offset + 16] = y1;
        vertices[offset + 17] = 0.0f;
        vertices[offset + 18] = 0.0f;
        vertices[offset + 19] = (float) textureSlot;

        vertices[offset + 20] = x1;
        vertices[offset + 21] = y0;
        vertices[offset + 22] = 1.0f;
        vertices[offset + 23] = 1.0f;
        vertices[offset + 24] = (float) textureSlot;

        vertices[offset + 25] = x1;
        vertices[offset + 26] = y1;
        vertices[offset + 27] = 1.0f;
        vertices[offset + 28] = 0.0f;
        vertices[offset + 29] = (float) textureSlot;

        // www.learnopengl.com:
        // "now advance cursors for next glyph
        // (note that advance is number of 1/64 pixels)
        // bitshift by 6 to get value in pixels (2^6 = 64)"
        advance = {advance.GetX() + ((glyph.advance) >> 6), advance.GetY()};
    }

    void Font::SetEscapeCharVertices(float* vertices, Math::Vec2f& advance, unsigned char escapeChar,
                                     unsigned int offset, unsigned int textureSlot)
    {

        switch(escapeChar) {
            // A tab character, which is equal to 4 spaces.
            case 't':
            {
                const unsigned char spaceChar = 32; // I think this is always true ...
                for(unsigned int i = 0; i < 4; i++)
                {
                    Glyph spaceGlyph = m_Characters[spaceChar];
                    SetCharVertices(vertices, advance, spaceGlyph, offset, textureSlot);
                }
                break;
            }
            // Newline character.
            case 'n': {
                advance = {0.0f, advance.GetY() + m_NewlineHeight};
                break;
            }
            default:
            {
                AW_ERROR("Escape character not recognized.");
                break;
            }
        }
    }
}