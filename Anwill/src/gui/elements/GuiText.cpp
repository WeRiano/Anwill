#include "GuiText.h"
#include "utils/Profiler.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    GuiText::GuiText(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                     unsigned int textSize, const std::shared_ptr<GuiStyling::Text>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::Text>() : style),
          m_TextPos(1.0f, -m_ContainerStyle->elementHeight / 2.0f + GuiStyling::Text::baselineOffset),
          m_Text(text),
          m_TextScale(GuiStyling::Text::font->GetScaleValue(textSize)),
          m_TextWidth((float) GuiStyling::Text::font->GetTextWidth(text) * m_TextScale)
    {}

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                         const Timestamp& delta)
    {
        Render(assignedPos, assignedMaxSize, delta, 0, (int) m_Text.length());
    }

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                         const Timestamp& delta, int startIndex, int length)
    {
        AW_PROFILE_FUNC();
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, m_TextPos + assignedPos);

        // Render
        std::string renderStr = m_Text.substr(startIndex, length);
        GuiStyling::Text::shader->Bind();
        Renderer2D::SubmitText(GuiStyling::Text::shader, *GuiStyling::Text::font, renderStr,
                               thisTransform);
    }

    bool GuiText::IsHovering(const Math::Vec2f& mousePos) const {
        return false;
    }

    float GuiText::GetWidth() const
    {
        return m_TextWidth;
    }

    float GuiText::GetWidth(unsigned char c) const
    {
        return m_TextWidth + GuiStyling::Text::font->GetGlyphWidth(c) * m_TextScale;
    }

    float GuiText::GetWidth(unsigned int startIndex, unsigned int size) const
    {
        std::string substr = m_Text.substr(startIndex, size);
        return (float)GuiStyling::Text::font->GetTextWidth(substr) * m_TextScale;
    }

    unsigned int GuiText::GetGridDepth() const
    {
        return 1;
    }

    void GuiText::Set(const std::string& text) {
        m_Text = text;
        m_TextWidth = (float) GuiStyling::Text::font->GetTextWidth(text) * m_TextScale;
    }

    void GuiText::AddCharacter(unsigned char c, unsigned int index)
    {
        m_Text.insert(index, std::string(1, c));
        m_TextWidth += GuiStyling::Text::font->GetGlyphWidth(c) * m_TextScale;
    }

    void GuiText::PrependCharacter(unsigned char c)
    {
        m_Text = std::string(1, (char) c) + m_Text;
        m_TextWidth += GuiStyling::Text::font->GetGlyphWidth(c) * m_TextScale;
    }

    void GuiText::AppendCharacter(unsigned char c)
    {
        m_Text.push_back((char) c);
        m_TextWidth += GuiStyling::Text::font->GetGlyphWidth(c) * m_TextScale;
    }

    char GuiText::RemoveCharacter(unsigned int characterIndex)
    {
        if(!m_Text.empty() && characterIndex >= 0 && characterIndex < m_Text.length()) {
            unsigned char removedChar = m_Text[characterIndex];
            m_Text.erase(characterIndex, 1);
            m_TextWidth -= GuiStyling::Text::font->GetGlyphWidth(removedChar) * m_TextScale;
            //AW_INFO("Glyph width: {0}", GuiStyling::Text::font->GetGlyphWidth(removedChar) * m_TextScale);
            return (char) removedChar;
        }
        return -1;
    }

    std::string GuiText::RemoveCharacters(unsigned int startCharacterIndex, unsigned int endCharacterIndex)
    {
        auto removedSubstr = m_Text.substr(startCharacterIndex, endCharacterIndex - startCharacterIndex);
        m_Text.erase(startCharacterIndex, endCharacterIndex - startCharacterIndex);
        m_TextWidth = (float) GuiStyling::Text::font->GetTextWidth(m_Text) * m_TextScale;
        return removedSubstr;
    }

    void GuiText::TruncateCharacter()
    {
        if(!m_Text.empty())
        {
            unsigned char c = m_Text.back();
            m_Text.pop_back();
            m_TextWidth -= GuiStyling::Text::font->GetGlyphWidth(c) * m_TextScale;
        }
    }

    unsigned char GuiText::PopCharacter()
    {
        if(!m_Text.empty())
        {
            unsigned char result = m_Text.front();
            m_Text.erase(m_Text.begin());
            m_TextWidth -= GuiStyling::Text::font->GetGlyphWidth(result) * m_TextScale;
            return result;
        }
        return 0;
    }

    std::string GuiText::ToString() const
    {
        return m_Text;
    }
}