#pragma once

#include "GuiElement.h"
#include "gui/styles/GuiStyling.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"
#include "core/Core.h"

namespace Anwill {

    class GuiText : public virtual GuiElement {
    public:
        Shared<GuiStyling::Text> m_Style;

        GuiText(const Shared<GuiStyling::Container>& containerStyle, const std::string& text,
                const Shared<GuiStyling::Text>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        float GetWidth(unsigned char c) const;
        float GetWidth(unsigned int startIndex, unsigned int size) const;
        unsigned int GetGridDepth() const override;

        void SetText(const std::string& text);
        std::string ToString() const;

    protected:
        // TODO: Is this necessary? Should it be owned by derived types instead?
        Math::Vec2f m_TextPos;
        std::string m_Text;
        float m_TextScale, m_TextWidth; // Calculate TextScale each iteration? Cheap.

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta, int startIndex, int endIndex);
        void AddCharacter(unsigned char c, unsigned int index);
        void PrependCharacter(unsigned char c);
        void AppendCharacter(unsigned char c);
        char RemoveCharacter(unsigned int characterIndex);
        std::string RemoveCharacters(unsigned int startCharacterIndex, unsigned int endCharacterIndex);
        /**
         * @brief Remove the rightmost char from the string.
         * @return The removed char.
         */
        unsigned char TruncateCharacter();
        /**
         * @brief Remove the leftmost char from the string.
         * @return The removed char.
         */
        unsigned char PopCharacter();
    };
}