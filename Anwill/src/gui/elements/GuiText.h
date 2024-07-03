#pragma once

#include "GuiElement.h"
#include "gui/styles/GuiStyling.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiText : public GuiElement {
    public:
        std::shared_ptr<GuiStyling::Text> m_Style;

        GuiText(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                unsigned int textSize, const std::shared_ptr<GuiStyling::Text>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta, int startIndex, int endIndex);
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        float GetWidth(unsigned char c) const;
        float GetWidth(unsigned int startIndex, unsigned int size) const;
        unsigned int GetGridDepth() const override;

        void Set(const std::string& text);
        void AddCharacter(unsigned char c, unsigned int index);
        void PrependCharacter(unsigned char c);
        void AppendCharacter(unsigned char c);
        char RemoveCharacter(unsigned int characterIndex);
        std::string RemoveCharacters(unsigned int startCharacterIndex, unsigned int endCharacterIndex);
        /**
         * @brief Remove the rightmost char from the string.
         */
        void TruncateCharacter();
        /**
         * @brief Remove the leftmost char from the string.
         * @return The popped char.
         */
        unsigned char PopCharacter();
        std::string ToString() const;

    protected:
        Math::Vec2f m_TextPos;
        std::string m_Text;
        float m_TextScale, m_TextWidth; // Calculate TextScale each iteration? Cheap.
    };
}