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

        FT_Set_Pixel_Sizes(ftFace, 0, 48);

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
            newGlyph.y0 = -newGlyph.height + newGlyph.bearingY;
            newGlyph.x1 = newGlyph.x0 + newGlyph.width;
            newGlyph.y1 = newGlyph.y0 + newGlyph.height;

            newGlyph.texture = Anwill::Texture::Create(ftFace->glyph->bitmap.buffer,
                                                       ftFace->glyph->bitmap.width,
                                                       ftFace->glyph->bitmap.rows);
            m_Characters.insert(std::pair<unsigned char, Glyph>(c, newGlyph));
        }
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLib);
    }

    int Font::Prepare(const std::string& text, const std::shared_ptr<Shader>& shader,
                        int startXPos)
    {
        // One glyph is 6 vertices
        // One vertex is 2 coordinates, 2 tex coordinates, 1 bitmap index
        // One glyph per text character
        unsigned int glyphSize = 6 * 5;
        auto vertices = new float[text.size() * glyphSize];
        int xAdvance = startXPos;

        std::map<unsigned char, unsigned int> textureSlotMap;
        unsigned int newTextSlotCount = 0;

        for(unsigned int i = 0; i < text.size(); i++)
        {
            unsigned char c = text[i];
            unsigned int thisIterationTextSlot;
            if (textureSlotMap.contains(c)) {
                // This character has already been used, lets find out which texture
                // slot it uses
                thisIterationTextSlot = textureSlotMap[c];
            } else {
                thisIterationTextSlot = newTextSlotCount;
                textureSlotMap[c] = thisIterationTextSlot;
                newTextSlotCount++;

                m_Characters[c].texture->Bind(thisIterationTextSlot);
                shader->Bind();
                shader->SetUniform1i(thisIterationTextSlot, "u_TextBitmaps[" +
                                     std::to_string(thisIterationTextSlot) + "]");
            }

            unsigned int offset = i * glyphSize;
            Glyph g = m_Characters[c];
            float x0 = g.x0 + xAdvance;
            float x1 = g.x1 + xAdvance;

            vertices[offset + 0] = x0;
            vertices[offset + 1] = g.y1;
            vertices[offset + 2] = 0.0f;
            vertices[offset + 3] = 0.0f;
            vertices[offset + 4] = (float) thisIterationTextSlot;

            vertices[offset + 5] = x0;
            vertices[offset + 6] = g.y0;
            vertices[offset + 7] = 0.0f;
            vertices[offset + 8] = 1.0f;
            vertices[offset + 9] = (float) thisIterationTextSlot;

            vertices[offset + 10] = x1;
            vertices[offset + 11] = g.y0;
            vertices[offset + 12] = 1.0f;
            vertices[offset + 13] = 1.0f;
            vertices[offset + 14] = (float) thisIterationTextSlot;

            vertices[offset + 15] = x0;
            vertices[offset + 16] = g.y1;
            vertices[offset + 17] = 0.0f;
            vertices[offset + 18] = 0.0f;
            vertices[offset + 19] = (float) thisIterationTextSlot;

            vertices[offset + 20] = x1;
            vertices[offset + 21] = g.y0;
            vertices[offset + 22] = 1.0f;
            vertices[offset + 23] = 1.0f;
            vertices[offset + 24] = (float) thisIterationTextSlot;

            vertices[offset + 25] = x1;
            vertices[offset + 26] = g.y1;
            vertices[offset + 27] = 1.0f;
            vertices[offset + 28] = 0.0f;
            vertices[offset + 29] = (float) thisIterationTextSlot;

            // www.learnopengl.com:
            // "now advance cursors for next glyph
            // (note that advance is number of 1/64 pixels)
            // bitshift by 6 to get value in pixels (2^6 = 64)"
            xAdvance += (g.advance >> 6);
        }

        m_VA = VertexArray::Create();
        m_VB = VertexBuffer::Create(vertices, sizeof(float) * text.size() * glyphSize);
        auto layout = BufferLayout({
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float2),
            BufferElement(ShaderDataType::Float)
        });

        m_VA->AddBuffer(*m_VB, layout);
        m_VA->Bind();
        m_VB->Bind();

        delete[] vertices;
        return xAdvance;
    }

    void Font::Done()
    {
        m_VA->Unbind();
        m_VB->Unbind();
    }



    /* DEPRECATED: See header.
    void Font::Bind(const unsigned char c)
    {
        Glyph g = m_Characters[c];

        // Bitmap starts top left, not bottom left.
        float x0 = g.x0 + m_Stats.advanceX;
        float x1 = g.x1 + m_Stats.advanceX;

        float vertices[6][4] = {
                {x0, g.y1, 0.0f, 0.0f },
                {x0, g.y0, 0.0f, 1.0f },
                {x1, g.y0, 1.0f, 1.0f },

                {x0, g.y1, 0.0f, 0.0f },
                {x1, g.y0, 1.0f, 1.0f },
                {x1, g.y1, 1.0f, 0.0f }
        };

        m_Stats.advanceX += (g.advance >> 6);

        m_Characters[c].texture->Bind();
        m_VB->DynamicUpdate(vertices, sizeof(vertices));
    }

    void Font::Unbind(const unsigned char c)
    {
        m_Characters[c].texture->Unbind();
    }
     */
}