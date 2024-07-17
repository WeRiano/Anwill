#include "GuiInputText.h"
#include "core/Log.h"
#include "core/Input.h"
#include "gfx/Renderer2D.h"
#include "math/Math.h"
#include "events/GuiEventHandler.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiInputText::GuiInputText(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                               const std::string& startText, float pixelWidth,
                               const std::shared_ptr<GuiStyling::InputText>& style)
        : GuiElement(containerStyle),
          GuiText(containerStyle, startText, AW_GUI_ENSURE_STYLE(style, GuiStyling::InputText)),
          GuiButton(containerStyle, {pixelWidth, containerStyle->elementHeight}, [](){},
                    AW_GUI_CAST_STYLE(GuiText::m_Style, GuiStyling::InputText)),
          m_Style(AW_GUI_CAST_STYLE(GuiText::m_Style, GuiStyling::InputText)),
          m_RenderLeftIndex(0), m_RenderRightIndex((int) startText.length()),
          m_SelectLeftIndex(0), m_SelectRightIndex(0),
          m_TimeCountMS(GuiStyling::InputText::cursorShowTimeIntervalMS), m_CursorIndex(0),
          m_ShowCursor(false)
    {
        m_Style->buttonHoverColor = m_Style->buttonColor;
        m_Style->buttonPressColor = m_Style->buttonColor;

        while(IsTextWiderThanBox())
        {
            m_RenderRightIndex--;
        }
    }

    void GuiInputText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        if(m_IsSelected)
        {
            CalcCursorTimeInterval(delta);
        }

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        Math::Vec2f offset = {GuiStyling::TextButton::textPadding + 2.0f,
                              -m_HostContainerStyle->elementHeight * 0.5f};
        if(m_IsSelected) {
            RenderSelected(assignedPos, offset);
            RenderText(assignedPos, assignedMaxSize, delta);
            if(m_ShowCursor)
            {
                RenderCursor(assignedPos, offset);
            }
        } else {
            // Render text
            RenderText(assignedPos, assignedMaxSize, delta);
        }
    }

    bool GuiInputText::IsHovering(const Math::Vec2f& mousePos) const
    {
        return GuiButton::IsHovering(mousePos);
    }

    float GuiInputText::GetWidth() const
    {
        return GuiButton::GetWidth();
    }

    unsigned int GuiInputText::GetGridDepth() const
    {
        return 1;
    }

    void GuiInputText::OnKeyPress(const KeyCode& keyCode)
    {
        m_TimeCountMS = 0;
        m_ShowCursor = true;
        KeycodeToAction(keyCode);
    }

    void GuiInputText::OnKeyRepeat(const KeyCode& keyCode)
    {
        m_TimeCountMS = 0;
        m_ShowCursor = true;
        KeycodeToAction(keyCode);
    }

    void GuiInputText::OnKeyChar(unsigned char c)
    {
        GuiText::AddCharacter(c, m_CursorIndex);
        m_CursorIndex++;
        m_RenderRightIndex++;
        if(m_SelectLeftIndex < m_SelectRightIndex)
        {
            RemoveSelectedCharacters();
        }
        ResetSelect();
        while(IsTextWiderThanBox())
        {
            // Shift the text to the left until it fits
            m_RenderLeftIndex++;
        }
    }

    void GuiInputText::RenderSelected(const Math::Vec2f& assignedPos, const Math::Vec2f& offset)
    {
        // Render "selected text box"
        if(m_SelectLeftIndex != m_SelectRightIndex)
        {
            // If we are selecting more than we are rendering, use the render indices instead
            int leftIndex = Math::Max(m_RenderLeftIndex, m_SelectLeftIndex);
            int rightIndex = Math::Min(m_RenderRightIndex, m_SelectRightIndex);
            // Get the text box start position (at left index)
            float selectedStartXPos = GuiText::GetWidth(0, leftIndex);
            // Adjust the start position if there is overflow from the left (render index is larger than 0)
            selectedStartXPos -= GuiText::GetWidth(0, m_RenderLeftIndex);
            // Get the width of the text box
            float selectedTextWidth = GuiText::GetWidth(leftIndex, rightIndex - leftIndex);
            Math::Vec2f size = {selectedTextWidth, m_HostContainerStyle->elementHeight - 4.0f};
            Math::Mat4f transform = Math::Mat4f::Scale({}, size);
            transform = Math::Mat4f::Translate(transform,
                                               {assignedPos + Math::Vec2f(selectedStartXPos, 0.0f) +
                                                Math::Vec2f(size.X * 0.5f, 0.0f)
                                                + offset});
            GuiStyling::primitiveShader->Bind();
            GuiStyling::primitiveShader->SetUniformVec3f(m_Style->selectedTextHighlightColor,"u_Color");
            Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::rectMesh, transform);
            GuiStyling::primitiveShader->Unbind();
        }
    }

    void GuiInputText::RenderText(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                                  const Timestamp& delta)
    {
        Math::Vec2f padding = Math::Vec2f(GuiStyling::TextButton::textPadding, 0.0f);
        int renderStrLength = m_RenderRightIndex - m_RenderLeftIndex;
        GuiText::Render(assignedPos + padding,assignedMaxSize - padding,
                      delta, m_RenderLeftIndex, renderStrLength);
    }

    void GuiInputText::RenderCursor(const Math::Vec2f& assignedPos, const Math::Vec2f& offset)
    {
        // Render cursor
        Math::Mat4f transform = Math::Mat4f::Scale({},
           {1.0f, m_Style->GetCursorHeight(m_HostContainerStyle->elementHeight), 0.0f});
        float cursorXPos = GuiText::GetWidth(0, m_CursorIndex);
        cursorXPos -= GuiText::GetWidth(0, m_RenderLeftIndex);
        transform = Math::Mat4f::Translate(transform, assignedPos
                                                      + Math::Vec2f(cursorXPos, 0.0f) + offset);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
        Renderer2D::SubmitLines(GuiStyling::primitiveShader, GuiStyling::InputText::cursorVertexArray,
                                transform, 1);
    }

    void GuiInputText::KeycodeToAction(const KeyCode& keyCode)
    {
        switch(keyCode) {
            case KeyCode::Backspace:
                DeleteText();
                break;
            case KeyCode::Enter:
                GuiEventHandler::Add(GuiLoseFocusEvent());
                break;
            case KeyCode::A:
                if(Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl))
                    SelectAll();
                break;
            case KeyCode::Right:
                MoveRight();
                break;
            case KeyCode::Left:
                MoveLeft();
                break;
            default:
                break;
        }
        //DebugIndices();
    }

    void GuiInputText::DeleteText()
    {
        if(m_SelectLeftIndex < m_SelectRightIndex)
        {
            RemoveSelectedCharacters();
        }
        else
        {
            RemoveCharacterAtCursor();
        }
        ResetSelect();
    }

    void GuiInputText::RefillOverflowFromLeft()
    {
        while(m_RenderLeftIndex > 0 &&
              !IsTextWiderThanBox(m_RenderLeftIndex - 1, m_RenderRightIndex))
        {
            m_RenderLeftIndex--;
        }
    }

    void GuiInputText::RefillOverflow()
    {
        bool left = true, right = true;
        while(left || right)
        {
            left = right = false;
            if (m_RenderLeftIndex > 0
                && !IsTextWiderThanBox(m_RenderLeftIndex - 1, m_RenderRightIndex))
            {
                m_RenderLeftIndex--;
                left = true;
            }
            if (m_RenderRightIndex < GuiText::ToString().length()
                && !IsTextWiderThanBox(m_RenderLeftIndex, m_RenderRightIndex + 1))
            {
                m_RenderRightIndex++;
                right = true;
            }
        }
    }

    void GuiInputText::RemoveSelectedCharacters()
    {
        /// 1. Remove selected substring from text
        GuiText::RemoveCharacters(m_SelectLeftIndex, m_SelectRightIndex);

        /// 2. Set cursor index to
        m_CursorIndex = m_SelectLeftIndex;

        /// 3. Set render indices to cursor index and expand accordingly
        m_RenderLeftIndex = m_RenderRightIndex = m_CursorIndex;
        RefillOverflow();
    }

    void GuiInputText::RemoveCharacterAtCursor()
    {
        if(GuiText::RemoveCharacter(m_CursorIndex - 1) == -1)
            return;
        m_CursorIndex--;
        m_RenderRightIndex = Math::Max(--m_RenderRightIndex, 0);
        RefillOverflowFromLeft();
    }

    void GuiInputText::ResetSelect()
    {
        m_SelectLeftIndex = m_SelectRightIndex = m_CursorIndex;
    }

    void GuiInputText::SelectAll()
    {
        m_SelectLeftIndex = 0;
        m_SelectRightIndex = (int) GuiText::ToString().length();
    }

    void GuiInputText::MoveRight()
    {
        // Move the cursor to the right
        m_CursorIndex = Math::Min(++m_CursorIndex, (int) GuiText::ToString().length());
        if(Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift)) {
            // If we are selecting ...
            if(m_CursorIndex > m_SelectRightIndex) {
                // ... expand to the right
                m_SelectRightIndex = Math::Min(++m_SelectRightIndex, (int) GuiText::ToString().length());
            } else if(m_CursorIndex < m_SelectRightIndex) {
                // ... or collapse to the right
                m_SelectLeftIndex++;
            }
        } else
        {
            // Not selecting text, so we reset any previous selection
            ResetSelect();
        }
        if(m_CursorIndex > m_RenderRightIndex && m_RenderRightIndex < GuiText::ToString().length())
        {
            // Horizontal scrolling of text in case of overflow
            m_RenderRightIndex++;
            while(IsTextWiderThanBox())
            {
                // Hide text until it fits
                m_RenderLeftIndex++;
            }
        }
    }

    void GuiInputText::MoveLeft()
    {
        // Move the cursor to the left
        m_CursorIndex = Math::Max(--m_CursorIndex, 0);
        if(Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift)) {
            // If we are selecting ...
            if(m_CursorIndex < m_SelectLeftIndex) {
                // ... expand to the left
                m_SelectLeftIndex = Math::Max(--m_SelectLeftIndex, 0);
            } else if(m_CursorIndex > m_SelectLeftIndex) {
                // ... or collapse to the left
                m_SelectRightIndex--;
            }
        } else
        {
            // Not selecting text, so we reset any previous selection
            ResetSelect();
        }
        if(m_CursorIndex < m_RenderLeftIndex && m_RenderLeftIndex > 0)
        {
            // Horizontal scrolling of text in case of overflow
            m_RenderLeftIndex--;
            while(IsTextWiderThanBox())
            {
                // Hide text until it fits
                m_RenderRightIndex--;
            }
        }
    }

    bool GuiInputText::IsTextWiderThanBox() const
    {
        int size = m_RenderRightIndex - m_RenderLeftIndex;
        return GuiText::GetWidth(m_RenderLeftIndex, size) >
               (GuiButton::GetWidth() - GuiStyling::TextButton::textPadding * 2.0f);
    }

    bool GuiInputText::IsTextWiderThanBox(int leftIndex, int rightIndex) const
    {
        if(GuiText::ToString().empty())
        {
            return false;
        }
        leftIndex = Math::Max(leftIndex, 0);
        rightIndex = Math::Min(rightIndex, (int) GuiText::ToString().length());
        int size = rightIndex - leftIndex;
        return GuiText::GetWidth(leftIndex, size) >
               (GuiButton::GetWidth() - GuiStyling::TextButton::textPadding * 2.0f);
    }

    void GuiInputText::CalcCursorTimeInterval(const Timestamp& delta)
    {
        m_TimeCountMS += delta.GetMilliseconds();
        if(m_TimeCountMS > GuiStyling::InputText::cursorShowTimeIntervalMS)
        {
            m_ShowCursor = !m_ShowCursor;
            m_TimeCountMS -= GuiStyling::InputText::cursorShowTimeIntervalMS;
        }
    }

    void GuiInputText::DebugIndices() const
    {
        AW_INFO("-------------------------");
        AW_INFO("Cursor: {0}", m_CursorIndex);
        AW_INFO("Render: {0}, {1}", m_RenderLeftIndex, m_RenderRightIndex);
        AW_INFO("Select: {0}, {1}", m_SelectLeftIndex, m_SelectRightIndex);
        AW_INFO("Text size: {0}", GuiText::ToString().length());
        AW_INFO("-------------------------");
    }
}