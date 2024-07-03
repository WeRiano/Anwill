#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiInputText : public GuiElement {
    public:
        std::shared_ptr<GuiStyling::InputText> m_Style;

        GuiInputText(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& startText,
                     unsigned int textSize, float pixelWidth,
                     const std::shared_ptr<GuiStyling::InputText>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        void OnKeyPress(const KeyCode& keyCode) override;
        void OnKeyRepeat(const KeyCode& keyCode) override;
        void OnKeyChar(unsigned char c) override;

    private:
        void RenderSelected(const Math::Vec2f& assignedPos, const Math::Vec2f& offset);
        void RenderText(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                        const Timestamp& delta);
        void RenderCursor(const Math::Vec2f& assignedPos, const Math::Vec2f& offset);

        void KeycodeToAction(const KeyCode& keyCode);
        void DeleteText();
        void RefillOverflowFromLeft();
        void RefillOverflow();
        void RemoveSelectedCharacters();
        void RemoveCharacterAtCursor();
        void ResetSelect();
        void SelectAll();
        void MoveRight();
        void MoveLeft();
        bool IsTextWiderThanBox() const;
        bool IsTextWiderThanBox(int leftIndex, int rightIndex) const;
        void CalcCursorTimeInterval(const Timestamp& delta);
        void DebugIndices() const;

        /// Underlying text and button
        GuiText m_Text;
        GuiButton m_Button;
        /// Indicates what part of the string to render
        /// The left index is equal to the number of characters that are hidden due to overflow
        int m_RenderLeftIndex, m_RenderRightIndex;
        /// Indicates what part of the string that is selected
        /// Can be larger than
        int m_SelectLeftIndex, m_SelectRightIndex;
        /// Counter used for 'blinking' cursor
        long double m_TimeCountMS;
        /// Indicates the string position of the cursor
        int m_CursorIndex;
        /// Indicates if the cursor should be rendered or not
        bool m_ShowCursor;
    };
}