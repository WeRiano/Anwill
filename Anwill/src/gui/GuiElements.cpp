#include <memory>

#include "core/Log.h"
#include "core/Input.h"
#include "events/GuiEvents.h"
#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "utils/Utils.h"
#include "utils/Profiler.h"

namespace Anwill {

    #pragma region Helpers

    /**
     * @brief Get the maximum allowed width and height of an element given its position and
     *        the current maximum width and height.
     */
    Math::Vec2f GetNewMaxSize(const Math::Vec2f& posDelta,
                              const Math::Vec2f& oldMaxSize) {
        return {oldMaxSize.X - posDelta.X, oldMaxSize.Y + posDelta.Y};
    }

    /**
     * @brief Get the position of the next horizontal or vertical element in a container
     */
    Math::Vec2f GetNextElementPos(const Math::Vec2f& curPos, float currentElementWidth,
                                  unsigned int currentElementGridDepth, float originXPos,
                                  bool onNewRow)
    {
        if(onNewRow) {
            return {
                    originXPos,
                    curPos.Y - ((float) currentElementGridDepth * (GuiStyling::Window::elementHeight +
                                                                        GuiStyling::Window::elementVerticalMargin))
            };
        } else {
            return {
                    curPos.X + currentElementWidth + GuiStyling::Window::elementHorizontalMargin,
                    curPos.Y
            };
        }
    }

    #pragma endregion

    #pragma region Icon

    void GuiIcon::RenderRightArrow(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.X, assignedSize.Y, 1.0f});
        iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.X, -assignedSize.Y));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::triangleMesh,
                               iconTransform);
    }

    void GuiIcon::RenderDownArrow(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.X, assignedSize.Y, 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.X, -assignedSize.Y));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::triangleMesh, iconTransform);
    }

    void GuiIcon::RenderCross(const Math::Vec2f& assignedPos,
                              const Math::Vec2f& assignedSize,
                              const Math::Vec3f& color) {
        // TODO
    }

    void GuiIcon::RenderCheckmark(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.X, assignedSize.Y, 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + (Math::Vec2f(assignedSize.X, -assignedSize.Y) * 0.5f));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::checkmarkMesh, iconTransform);
    }

    void GuiIcon::RenderRectangle(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.X,
                                                                             -assignedSize.Y) * 0.5f));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::rectMesh, iconTransform);
    }

    void GuiIcon::RenderEllipse(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.X,
                                                                             -assignedSize.Y) * 0.5f));

        GuiStyling::circleShader->Bind();
        GuiStyling::circleShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::circleShader, GuiStyling::rectMesh, iconTransform);
    }

    typedef std::function<void(const Math::Vec2f&, const Math::Vec2f&, const Math::Vec3f&)> RenderIconFunction;
    static std::array<RenderIconFunction,
                      (size_t) GuiStyling::Checkbox::CheckmarkType::Size> renderIconFunctions = {
            GuiIcon::RenderCheckmark,
            GuiIcon::RenderRectangle,
            GuiIcon::RenderEllipse
    };

    #pragma endregion

    #pragma region Tooltip

    GuiTooltip::GuiTooltip(const std::string& tooltipText, unsigned int tooltipTextSize)
                           : m_TooltipText(tooltipText),
                             m_TooltipTextScale(GuiStyling::Text::font->GetScaleValue(tooltipTextSize))
    {}

    void GuiTooltip::Render(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize)
    {
        AW_PROFILE_FUNC();
        // First render the tooltip background window
        Math::Vec2f backgroundTopLeftCorner = mousePos + Math::Vec2f(GuiStyling::Tooltip::offset, 0.0f);
        Math::Vec2f textStartPos = backgroundTopLeftCorner +
                Math::Vec2f(GuiStyling::Tooltip::windowMargin.X, -GuiStyling::Tooltip::windowMargin.Y) +
                            Math::Vec2f(0.0f, -GuiStyling::Text::font->GetFontHeight()) * m_TooltipTextScale;

        Math::Vec2f textSize = GuiStyling::Text::font->GetTextSize(m_TooltipText) * m_TooltipTextScale;
        Math::Vec2f backgroundSize = textSize + GuiStyling::Tooltip::windowMargin * 2.0f;

        // Move to fit inside game window
        bool isOutsideX = backgroundTopLeftCorner.X + backgroundSize.X > gameWindowSize.X;
        bool isOutsideY = backgroundTopLeftCorner.Y - backgroundSize.Y < 0.0f;
        Math::Vec2f correctedOffset;
        if(isOutsideX and isOutsideY) {
            float correctedOffsetX = gameWindowSize.X -
                                     (backgroundTopLeftCorner.X + backgroundSize.X);
            correctedOffset = {correctedOffsetX, GuiStyling::Tooltip::offset + backgroundSize.Y};
        } else if (isOutsideX) {
            float correctedOffsetX = gameWindowSize.X -
                    (backgroundTopLeftCorner.X + backgroundSize.X);
            correctedOffset = {correctedOffsetX, -GuiStyling::Tooltip::offset};
        } else if (isOutsideY) {
            float correctedOffsetY = -(backgroundTopLeftCorner.Y - backgroundSize.Y);
            correctedOffset = {0.0f, correctedOffsetY};
        }

        Math::Mat4f transform = Math::Mat4f::Scale({}, backgroundSize);
        transform = Math::Mat4f::Translate(transform, backgroundTopLeftCorner + correctedOffset +
                                Math::Vec2f(backgroundSize.X, -backgroundSize.Y) * 0.5f);
        Renderer2D::SubmitMesh(GuiStyling::Tooltip::shader, GuiStyling::rectMesh, transform);

        // Then render the tooltip text
        transform = Math::Mat4f::Scale({}, {m_TooltipTextScale, m_TooltipTextScale, 0.0f});
        transform = Math::Mat4f::Translate(transform, textStartPos + correctedOffset);
        Renderer2D::SubmitText(GuiStyling::Text::shader, *GuiStyling::Text::font, m_TooltipText,
                               transform);
    }

    #pragma endregion

    #pragma region Element

    GuiElement::GuiElement()
        : m_IsHovered(false), m_IsPressed(false), m_IsSelected(false)
    {}

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::OnHover(const Math::Vec2f& mousePos) {}

    void GuiElement::StartPressing()
    {
        m_IsPressed = true;
    }

    void GuiElement::OnPress(const Math::Vec2f& mousePos)
    {}

    void GuiElement::Release()
    {
        m_IsPressed = false;
    }

    void GuiElement::Select()
    {
        m_IsSelected = true;
    }

    void GuiElement::Deselect()
    {
        m_IsSelected = false;
    }

    void GuiElement::OnKeyPress(const KeyCode& keyCode) {}
    void GuiElement::OnKeyRepeat(const KeyCode& keyCode) {}
    void GuiElement::OnKeyRelease(const KeyCode& keyCode) {}
    void GuiElement::OnKeyChar(unsigned char) {}

    void GuiElement::OnHoverRender(const Math::Vec2f& mousePos, const Math::Vec2f& gameWindowSize)
    {
        if(m_Tooltip != nullptr)
        {
            m_Tooltip->Render(mousePos, gameWindowSize);
        }
    }

    void GuiElement::EmplaceTooltip(const std::string& tooltipText)
    {
        m_Tooltip = std::make_unique<GuiTooltip>(tooltipText, GuiStyling::Text::fontSize);
    }

    #pragma endregion

    #pragma region Text

    GuiText::GuiText(const std::string& text, unsigned int textSize)
        : GuiElement(),
          m_TextPos(1.0f, -GuiStyling::Window::elementHeight / 2.0f + GuiStyling::Text::baselineOffset),
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

    #pragma endregion

    #pragma region Button

    GuiButton::GuiButton(const Math::Vec2f& size, const std::function<void()>& callback)
        : GuiElement(),
          m_ButtonSize(size),
          m_Callback(callback)
    {}

    void GuiButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                           const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_ButtonSize);
        thisTransform = Math::Mat4f::Translate(thisTransform,
                                               assignedPos + Math::Vec2f(m_ButtonSize.X / 2.0f,
                                                                         -m_ButtonSize.Y / 2.0f));

        // Render button
        auto shader = GuiStyling::Button::shaders[(std::size_t) m_ButtonStyle.buttonShape];
        shader->Bind();
        shader->SetUniform1i(m_IsHovered, "u_Hovering");
        shader->SetUniform1i(m_IsPressed, "u_Pressing");
        shader->SetUniformVec3f(m_ButtonStyle.buttonColor, "u_Color");
        shader->SetUniformVec3f(m_ButtonStyle.buttonHoverColor, "u_HoverColor");
        shader->SetUniformVec3f(m_ButtonStyle.buttonPressColor, "u_PressColor");
        Renderer2D::SubmitMesh(shader, GuiStyling::rectMesh, thisTransform);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return Math::Algo::IsPointInsideRectangle({0.0f,                 0.0f},
                                                  {m_ButtonSize.X,  0.0f},
                                                  {m_ButtonSize.X, -m_ButtonSize.Y},
                                                  {0.0f,                -m_ButtonSize.Y},
                                                  mousePos);
    }

    float GuiButton::GetWidth() const
    {
        return m_ButtonSize.X;
    }

    unsigned int GuiButton::GetGridDepth() const
    {
        return 1;
    }

    void GuiButton::Release()
    {
        GuiElement::Release();
        m_Callback();
    }

    void GuiButton::SetCallback(const std::function<void()>& callback) {
        m_Callback = callback;
    }

    void GuiButton::SetWidth(float width)
    {
        m_ButtonSize = {width, m_ButtonSize.Y};
    }

    void GuiButton::SetHeight(float height)
    {
        m_ButtonSize = {m_ButtonSize.X, height};
    }

    #pragma endregion

    #pragma region TextButton

    GuiTextButton::GuiTextButton(const std::string& text, unsigned int textSize,
                                 const std::function<void()>& callback)
        : GuiButton({}, callback),
          m_Text(text, textSize)
    {
        m_ButtonSize = {m_Text.GetWidth() + GuiStyling::TextButton::textPadding * 2.0f,
                        GuiStyling::Window::elementHeight};
    }

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                               const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        // Render text
        Math::Vec2f padding = Math::Vec2f(GuiStyling::TextButton::textPadding, 0.0f);
        m_Text.Render(assignedPos + padding,
                      assignedMaxSize - padding,
                      delta);
    }

    void GuiTextButton::SetText(const std::string& text) {
        m_Text.Set(text);
        m_ButtonSize = { m_Text.GetWidth() + GuiStyling::TextButton::textPadding * 2.0f, m_ButtonSize.Y };
    }

    #pragma endregion

    #pragma region Checkbox

    GuiCheckbox::GuiCheckbox(bool checked, const std::string& text, unsigned int textSize,
                             const std::function<void(bool)>& callback)
        : GuiButton({GuiStyling::Window::elementHeight, GuiStyling::Window::elementHeight},
                    [this, callback](){
            m_Checked = !m_Checked;
            callback(m_Checked);
        }),
          m_Text(text, textSize),
          m_Checked(checked)
    {}

    void GuiCheckbox::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        // Render text
        Math::Vec2f textPosDelta = {m_ButtonSize.X + GuiStyling::Checkbox::textMargin, 0.0f};
        m_Text.Render(assignedPos + textPosDelta, assignedMaxSize - textPosDelta, delta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        // Render checkmark if it checked
        if(!m_Checked) { return; }
        Math::Vec2f margin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        RenderIconFunction renderFunc = renderIconFunctions[(size_t) m_CheckboxStyle.checkmarkType];
        renderFunc(assignedPos + Math::Vec2f(margin.X, -margin.Y),
                                 m_ButtonSize - margin * 2.0f,
                                 m_CheckboxStyle.checkmarkColor);
    }

    float GuiCheckbox::GetWidth() const
    {
        return GuiButton::GetWidth() + GuiStyling::Checkbox::textMargin + m_Text.GetWidth();
    }

    #pragma endregion

    #pragma region RadioButton

    GuiRadioButton::GuiRadioButton(const std::string& text, unsigned int textSize, int& reference,
                                   const int onSelectValue, const std::function<void()>& callback)
            : GuiButton({GuiStyling::Window::elementHeight, GuiStyling::Window::elementHeight},
                        [this, callback](){
                            if(m_Reference != m_OnSelectValue) {
                                callback();
                                m_Reference = m_OnSelectValue;
                            }
                        }),
              m_Text(text, textSize),
              m_Reference(reference),
              m_OnSelectValue(onSelectValue)
    {
        m_ButtonStyle.buttonShape = GuiStyling::Button::Shape::Ellipse;
    }

    void GuiRadioButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                                const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        Math::Vec2f textPosDelta = {m_ButtonSize.X + GuiStyling::Checkbox::textMargin, 0.0f};
        m_Text.Render(assignedPos + textPosDelta, assignedMaxSize - textPosDelta, delta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        // Render checkmark if radiobutton is selected
        if(m_Reference != m_OnSelectValue) { return; }
        Math::Vec2f margin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        GuiIcon::RenderEllipse(assignedPos + Math::Vec2f(margin.X, -margin.Y),
                   m_ButtonSize - margin * 2.0f,
                   m_RadioButtonStyle.checkmarkColor);
    }

    float GuiRadioButton::GetWidth() const
    {
        return GuiButton::GetWidth() + GuiStyling::Checkbox::textMargin + m_Text.GetWidth();
    }

    #pragma endregion

    #pragma region InputText

    GuiInputText::GuiInputText(const std::string& startText, unsigned int textSize, float pixelWidth)
        : GuiTextButton(startText, textSize, [](){}),
          m_RenderLeftIndex(0), m_RenderRightIndex(startText.length()),
          m_SelectLeftIndex(0), m_SelectRightIndex(0),
          m_TimeCountMS(0), m_CursorIndex(0), m_ShowCursor(false)
    {
        m_ButtonSize = {pixelWidth, m_ButtonSize.Y};
        m_ButtonStyle.buttonHoverColor = m_ButtonStyle.buttonColor;
        m_ButtonStyle.buttonPressColor = m_ButtonStyle.buttonColor;
    }

    void GuiInputText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        CalcCursorTimeInterval(delta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        Math::Vec2f offset = {GuiStyling::TextButton::textPadding + 2.0f,
                              -GuiStyling::Window::elementHeight * 0.5f};
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
        m_Text.AddCharacter(c, m_CursorIndex);
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
            float selectedStartXPos = m_Text.GetWidth(0, leftIndex);
            // Adjust the start position if there is overflow from the left (render index is larger than 0)
            selectedStartXPos -= m_Text.GetWidth(0, m_RenderLeftIndex);
            // Get the width of the text box
            float selectedTextWidth = m_Text.GetWidth(leftIndex, rightIndex - leftIndex);
            Math::Vec2f size = {selectedTextWidth, GuiStyling::Window::elementHeight - 2.0f};
            Math::Mat4f transform = Math::Mat4f::Scale({}, size);
            transform = Math::Mat4f::Translate(transform,
                                               {assignedPos + Math::Vec2f(selectedStartXPos, 0.0f) +
                                                Math::Vec2f(size.X * 0.5f, 0.0f)
                                                + offset});
            GuiStyling::primitiveShader->SetUniformVec3f(m_InputTextStyle.selectedTextHighlightColor,
                                                         "u_Color");
            Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::rectMesh, transform);
        }
    }

    void GuiInputText::RenderText(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                                  const Timestamp& delta)
    {
        Math::Vec2f padding = Math::Vec2f(GuiStyling::TextButton::textPadding, 0.0f);
        int renderStrLength = m_RenderRightIndex - m_RenderLeftIndex;
        m_Text.Render(assignedPos + padding,assignedMaxSize - padding,
                      delta, m_RenderLeftIndex, renderStrLength);
    }

    void GuiInputText::RenderCursor(const Math::Vec2f& assignedPos, const Math::Vec2f& offset)
    {
        // Render cursor
        Math::Mat4f transform = Math::Mat4f::Scale({}, {1.0f, GuiStyling::Text::cursorHeight, 0.0f});
        float cursorXPos = m_Text.GetWidth(0, m_CursorIndex);
        cursorXPos -= m_Text.GetWidth(0, m_RenderLeftIndex);
        transform = Math::Mat4f::Translate(transform, assignedPos
                                                      + Math::Vec2f(cursorXPos, 0.0f) + offset);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f({1.0f, 1.0f, 1.0f}, "u_Color");
        Renderer2D::SubmitLines(GuiStyling::primitiveShader, GuiStyling::Text::cursorVertexArray,
                                transform, 1);
    }

    void GuiInputText::KeycodeToAction(const KeyCode& keyCode)
    {
        switch(keyCode) {
            case KeyCode::Backspace:
                DeleteText();
                break;
            case KeyCode::Enter:
                GuiEvents::Add(GuiLoseFocusEvent());
                break;
            case KeyCode::A:
                if(Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl))
                    SelectAll();
                    break;
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
            if (m_RenderRightIndex < m_Text.ToString().length()
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
        m_Text.RemoveCharacters(m_SelectLeftIndex, m_SelectRightIndex);

        /// 2. Set cursor index to
        m_CursorIndex = m_SelectLeftIndex;

        /// 3. Set render indices to cursor index and expand accordingly
        m_RenderLeftIndex = m_RenderRightIndex = m_CursorIndex;
        RefillOverflow();
    }

    void GuiInputText::RemoveCharacterAtCursor()
    {
        if(m_Text.RemoveCharacter(m_CursorIndex - 1) == -1)
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
        m_SelectRightIndex = (int) m_Text.ToString().length();
    }

    void GuiInputText::MoveRight()
    {
        // Move the cursor to the right
        m_CursorIndex = Math::Min(++m_CursorIndex, (int) m_Text.ToString().length());
        if(Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift)) {
            // If we are selecting ...
            if(m_CursorIndex > m_SelectRightIndex) {
                // ... expand to the right
                m_SelectRightIndex = Math::Min(++m_SelectRightIndex, (int) m_Text.ToString().length());
            } else if(m_CursorIndex < m_SelectRightIndex) {
                // ... or collapse to the right
                m_SelectLeftIndex++;
            }
        } else
        {
            // Not selecting text, so we reset any previous selection
            ResetSelect();
        }
        if(m_CursorIndex > m_RenderRightIndex && m_RenderRightIndex < m_Text.ToString().length())
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
        return m_Text.GetWidth(m_RenderLeftIndex, size) >
               (m_ButtonSize.X - GuiStyling::TextButton::textPadding * 2.0f);
    }

    bool GuiInputText::IsTextWiderThanBox(int leftIndex, int rightIndex) const
    {
        if(m_Text.ToString().empty())
        {
            return false;
        }
        leftIndex = Math::Max(leftIndex, 0);
        rightIndex = Math::Min(rightIndex, (int) m_Text.ToString().length());
        int size = rightIndex - leftIndex;
        return m_Text.GetWidth(leftIndex, size) >
               (m_ButtonSize.X - GuiStyling::TextButton::textPadding * 2.0f);
    }

    void GuiInputText::CalcCursorTimeInterval(const Timestamp& delta)
    {
        m_TimeCountMS += delta.GetMilliseconds();
        if(m_TimeCountMS > GuiStyling::Text::cursorShowTimeIntervalMS)
        {
            m_ShowCursor = !m_ShowCursor;
            m_TimeCountMS -= GuiStyling::Text::cursorShowTimeIntervalMS;
        }
    }

    void GuiInputText::DebugIndices() const
    {
        AW_INFO("-------------------------");
        AW_INFO("Cursor: {0}", m_CursorIndex);
        AW_INFO("Render: {0}, {1}", m_RenderLeftIndex, m_RenderRightIndex);
        AW_INFO("Select: {0}, {1}", m_SelectLeftIndex, m_SelectRightIndex);
        AW_INFO("Text size: {0}", m_Text.ToString().length());
        AW_INFO("-------------------------");
    }

    #pragma endregion

    #pragma region Image

    GuiImage::GuiImage(const std::string& fileName, unsigned int maxRows)
        : m_Texture(Texture::Create(fileName))
    {
        if(m_Texture->GetHeight() > maxRows * AW_GUI_WINDOW_ROW_HEIGHT) {
            m_ScaleFactor = maxRows * AW_GUI_WINDOW_ROW_HEIGHT / m_Texture->GetHeight();
        } else {
            m_ScaleFactor = 1.0f;
        }
        m_GridDepth = std::ceil(m_Texture->GetHeight() * m_ScaleFactor / AW_GUI_WINDOW_ROW_HEIGHT);
    }

    void GuiImage::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                          const Timestamp& delta)
    {
        AW_PROFILE_FUNC();

        Math::Vec2f imageSize = {static_cast<float>(m_Texture->GetWidth()),
                                 static_cast<float>(m_Texture->GetHeight())};
        imageSize = imageSize * m_ScaleFactor;
        Math::Mat4f transform = Math::Mat4f::Scale({}, imageSize);
        transform = Math::Mat4f::Translate(transform, {assignedPos.X + imageSize.X * 0.5f,
                                                       assignedPos.Y - imageSize.Y * 0.5f,
                                                       0.0f});
        Renderer2D::SubmitMesh(GuiStyling::Image::shader, Mesh::GetUnitRectangle(true),
                               transform, m_Texture);
    }

    bool GuiImage::IsHovering(const Math::Vec2f& mousePos) const {
        return Math::Algo::IsPointInsideRectangle({0.0f, 0.0f},
                                                  {static_cast<float>(m_Texture->GetWidth() * m_ScaleFactor), 0.0f},
                                                  {static_cast<float>(m_Texture->GetWidth() * m_ScaleFactor),
                                                   -static_cast<float>(m_Texture->GetHeight() * m_ScaleFactor)},
                                                  {0.0f, -static_cast<float>(m_Texture->GetHeight())},
                                                  mousePos);
    }

    float GuiImage::GetWidth() const {
        return m_Texture->GetWidth() * m_ScaleFactor;
    }

    unsigned int GuiImage::GetGridDepth() const {
        return m_GridDepth;
    }

    #pragma endregion

    #pragma region Container

    GuiContainer::GuiContainer()
            : m_Scrollbar({10.0f, 10.0f}, [](){}), m_ScrollOffset(), m_CanScroll(false),
              m_GridDepth(0), m_ContainerElements(), m_HideElements(true)
    {
        m_Scrollbar.m_ButtonStyle.buttonColor = {0.3f, 0.3f, 0.3f};
    }

    std::shared_ptr<GuiElement> GuiContainer::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                              const Math::Vec2f& mousePos) const
    {
        if(m_HideElements) { return nullptr; }
        for(unsigned int i = 0; i < m_ContainerElements.size(); i++) {
            auto& containerElement = m_ContainerElements[i];
            if (containerElement.element->IsHovering(mousePos - containerElement.position)) {
                hoverElementPos = containerElement.position;
                return containerElement.element;
            }
            if (dynamic_cast<GuiContainer*>(containerElement.element.get()) != nullptr) {
                // If element is a container we need to check with those elements
                auto container =
                    std::dynamic_pointer_cast<GuiContainer>(containerElement.element);
                Math::Vec2f elementPosInsideContainer = {};
                auto maybeResult = container->GetHoverElement(
                        elementPosInsideContainer,
                        mousePos - containerElement.position);
                if (maybeResult != nullptr) {
                    // If we found an element we return it, otherwise we continue
                    hoverElementPos = containerElement.position + elementPosInsideContainer;
                    return maybeResult;
                }
            }
        }
        return nullptr;
    }

    void GuiContainer::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Timestamp& delta)
    {
        // TODO: Cleanup, big function
        Renderer::PushScissor({assignedPos.X, assignedPos.Y - assignedMaxSize.Y}, assignedMaxSize);
        Math::Vec2f elementPos = m_ScrollOffset;
        float newRowXPos = elementPos.X;
        std::shared_ptr<GuiElement> lastElement = nullptr;
        for (unsigned int i = 0; i < m_ContainerElements.size(); i++) {
            auto& containerElement = m_ContainerElements[i];
            if(i > 0)
            {
                // Any element other than the first needs to know if it's on a new row or not
                // and then have its position calculated
                bool wantsNewRow = m_ContainerElements[i].onNewRow;
                // wrong? Should be bool from prev element
                bool forcedToNewRow = m_ContainerElements[i - 1].forceNextToNewRow;
                elementPos = GetNextElementPos(elementPos,
                                               lastElement->GetWidth(),
                                               lastElement->GetGridDepth(),
                                               newRowXPos,
                                               wantsNewRow || forcedToNewRow);
            }
            // Only render the element if it is inside the
            if(elementPos.Abs() >= assignedMaxSize)
            {
                m_ContainerElements[i].isHidden = true;
            } else
            {

                m_ContainerElements[i].isHidden = false;
                containerElement.element->Render(assignedPos + elementPos,
                                                 GetNewMaxSize(elementPos, assignedMaxSize),
                                                 delta);
            }
            containerElement.position = elementPos;
            lastElement = containerElement.element;
        }
        Renderer::EndScissor();
    }

    bool GuiContainer::IsHidingElements() const
    {
        return m_HideElements;
    }

    void GuiContainer::ScrollUp()
    {
        if(!m_CanScroll)
        {
            return;
        }
        m_ScrollOffset.Y = Math::Max(m_ScrollOffset.Y - s_ScrollSpeed, 0.0f);
    }

    void GuiContainer::ScrollDown()
    {
        if(!m_CanScroll)
        {
            return;
        }
        // TODO: Has to include the cutoff distance here.
        m_ScrollOffset.Y = Math::Min(m_ScrollOffset.Y + s_ScrollSpeed, m_HiddenSize.Y);
    }

    void GuiContainer::RenderVerticalScrollbar(const Math::Vec2f& assignedPos, float visibleHeight,
                                               const Timestamp& delta)
    {
        float contentHeight = (float)m_GridDepth * AW_GUI_WINDOW_ROW_HEIGHT;
        // How much of the window is hidden due to content overflow
        float hiddenHeight = Math::Max(contentHeight - visibleHeight, 0.0f);
        m_HiddenSize.Y = hiddenHeight;
        if(hiddenHeight <= 0.0f) {
        // Can only scroll if there is some hidden content
            m_CanScroll = false;
            return;
        }

        m_CanScroll = true;
        float scrollbarHeight = (visibleHeight * visibleHeight) / contentHeight;
        float emptyHeight = (visibleHeight - scrollbarHeight);
        // This coefficient represents how "far" the user has scrolled into hidden territory (and will be on [0, 1])
        float scrollDistNorm = m_ScrollOffset.Y / m_HiddenSize.Y;
        Math::Vec2f scrollbarPosOffset = Math::Vec2f(0.0f, emptyHeight * scrollDistNorm);
        m_Scrollbar.SetHeight(scrollbarHeight);
        // TODO: Max size unused
        m_Scrollbar.Render(assignedPos - scrollbarPosOffset, {9999.9f, 9999.9f},
                           delta);
    }

    #pragma endregion

    #pragma region Dropdown

    GuiDropdown::GuiDropdown(const std::string& text, unsigned int textSize)
            : GuiTextButton(text, textSize, [this](){
        m_HideElements = !m_HideElements;
    })
    {
        m_GridDepth++;
    }

    std::shared_ptr<GuiElement> GuiDropdown::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                             const Math::Vec2f& mousePos) const
    {
        return GuiContainer::GetHoverElement(hoverElementPos,
                                             mousePos - GuiStyling::Dropdown::elementStartPos);
    }

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        // Set button size to the maximum allowed width
        m_ButtonSize = {assignedMaxSize.X, m_ButtonSize.Y};
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        // Render arrow icon
        if(m_HideElements) {
            GuiIcon::RenderRightArrow(assignedPos,
                                      GuiStyling::iconSize * 0.5f,
                                      GuiStyling::iconColor);
        } else {
            GuiIcon::RenderDownArrow(assignedPos,
                                     GuiStyling::iconSize * 0.5f,
                                     GuiStyling::iconColor);
        }

        // Render text slightly to the right compared to a regular text button
        m_Text.Render(assignedPos + Math::Vec2f(GuiStyling::iconSize.X, 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiStyling::iconSize.X, 0.0f), delta);

        if(m_HideElements) { return; }
        GuiContainer::Render(assignedPos + Math::Vec2f(GuiStyling::Dropdown::elementIndent,-AW_GUI_WINDOW_ROW_HEIGHT),
                             assignedMaxSize, delta);
    }

    bool GuiDropdown::IsHovering(const Math::Vec2f& mousePos) const
    {
        return GuiTextButton::IsHovering(mousePos);
        /*
        if (GuiTextButton::IsHovering(mousePos)) {
            return true;
        }
        else {
            for(const auto& containerElement : m_ContainerElements) {
                if(containerElement.element->IsHovering(mousePos)) {
                    return false;
                }
            }
        }
        return false;
         */
    }

    float GuiDropdown::GetWidth() const
    {
        return GuiTextButton::GetWidth();
    }

    unsigned int GuiDropdown::GetGridDepth() const
    {
        return m_HideElements ? 1 : m_GridDepth;
    }

    #pragma endregion

    #pragma region Window

    GuiWindow::GuiWindow(const std::string& title, GuiWindowID id, const Math::Vec2f& position, const Math::Vec2f& size)
            : GuiContainer(), m_Pos(position), m_Size(size), m_LastShowSize(), m_ID(id),
              m_Title(title, 14),
              m_MinimizeButton(std::make_shared<GuiButton>(GuiStyling::iconSize,
                                                           [this]() {
                                                               m_HideElements = !m_HideElements;
                                                               if(m_HideElements) {
                                                                   m_LastShowSize = m_Size;
                                                                   m_Size = {m_Size.X,
                                                                             GuiStyling::Window::headerSize};
                                                               } else {
                                                                   m_Size = m_LastShowSize;
                                                               }
                                                           }))
    {
        m_HideElements = false;
    }

    std::shared_ptr<GuiElement> GuiWindow::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                           const Math::Vec2f& mousePos) const
    {
        // Two things in addition to GuiContainer implementation:
        //  1. We have a static minimize button element that we have to check for hovering
        //  2. We have to consider the position of the window since it is the "outer" most container
        if(m_MinimizeButton->IsHovering(mousePos - m_Pos)) {
            return m_MinimizeButton;
        }
        auto hoverElement = GuiContainer::GetHoverElement(hoverElementPos,
                                                  mousePos - m_Pos - GuiStyling::Window::elementStartPos);
        hoverElementPos = m_Pos + GuiStyling::Window::elementStartPos;
        return hoverElement;
    }

    void GuiWindow::Render(bool isSelected, const Timestamp& delta)
    {
        // TODO: Cleanup, big function
        Renderer::PushScissor({m_Pos.X, m_Pos.Y - m_Size.Y}, m_Size);
        // Render window
        GuiStyling::Window::shader->Bind();
        GuiStyling::Window::shader->SetUniform1i(isSelected, "u_Selected");
        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(),
                                                m_Pos + Math::Vec2f(m_Size.X / 2.0f, -m_Size.Y / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::SubmitMesh(GuiStyling::Window::shader, GuiStyling::rectMesh, transform);

        // Render title
        m_Title.Render(m_Pos + GuiStyling::Window::titlePos,
                       m_Size - GuiStyling::Window::titlePos - Math::Vec2f(GuiStyling::Window::cutoffPadding,
                                                                           GuiStyling::Window::cutoffPadding),
                                                                           delta);
        // Render minimize button
        m_MinimizeButton->Render(m_Pos, m_Size, delta);
        if(m_HideElements) {
            GuiIcon::RenderRightArrow(m_Pos,
                                      GuiStyling::iconSize * 0.5f,
                                      GuiStyling::iconColor);
        } else {
            GuiIcon::RenderDownArrow(m_Pos, GuiStyling::iconSize * 0.5f, GuiStyling::iconColor);

            // Render elements inside window
            GuiContainer::Render(m_Pos + GuiStyling::Window::elementStartPos,
                                 m_Size - GuiStyling::Window::elementStartPos.NegateY()
                                 - Math::Vec2f(GuiStyling::Window::cutoffPadding, GuiStyling::Window::cutoffPadding),
                                 delta);

            // Render scrollbar
            Math::Vec2f scrollBarPos = {m_Pos.X + m_Size.X - m_Scrollbar.GetWidth() - 3.0f,
                                        m_Pos.Y + -GuiStyling::Window::headerSize - 4.0f};
            RenderVerticalScrollbar(scrollBarPos, m_Size.Y - GuiStyling::Window::headerSize, delta);
        }
        Renderer::EndScissor();
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.X + GuiStyling::Window::titlePos.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y - GuiStyling::Window::headerSize},
                                                  {m_Pos.X + GuiStyling::Window::titlePos.X, m_Pos.Y - GuiStyling::Window::headerSize},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringResize(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideCircle({m_Pos.X + m_Size.X, m_Pos.Y - m_Size.Y},
                                               10.0f, mousePos);
    }

    bool GuiWindow::IsHoveringWindow(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y},
                                                  {m_Pos.X + m_Size.X, m_Pos.Y - m_Size.Y},
                                                  {m_Pos.X, m_Pos.Y - m_Size.Y},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringMinimize(const Math::Vec2f& mousePos) {
        return Math::Algo::IsPointInsideRectangle({m_Pos.X, m_Pos.Y},
                                                  {m_Pos.X + GuiStyling::iconSize.X, m_Pos.Y},
                                                  {m_Pos.X + GuiStyling::iconSize.X, m_Pos.Y
                                                  - GuiStyling::Window::headerSize},
                                                  {m_Pos.X, m_Pos.Y - GuiStyling::Window::headerSize},
                                                  mousePos);
    }

    void GuiWindow::Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos)
    {
        if(!(m_Pos.Y + delta.Y > maxPos.Y ||
             m_Pos.Y - GuiStyling::Window::headerSize + delta.Y < minPos.Y)) {
            m_Pos = {m_Pos.X, m_Pos.Y + delta.Y};
        }
        if(!(m_Pos.X + delta.X < minPos.X ||
             m_Pos.X + m_Size.X + delta.X > maxPos.X)) {
            m_Pos = {m_Pos.X + delta.X, m_Pos.Y};
        }
    }

    void GuiWindow::Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize)
    {
        Math::Vec2f newSize = m_Size + delta;
        newSize.Clamp(minSize, maxSize);
        m_Size = newSize;

        if(m_ScrollOffset.Y >= 0.0f && m_HiddenSize.Y != 0.0f && m_HiddenSize.Y <= m_ScrollOffset.Y) {
            m_ScrollOffset.Y = Math::Max(m_ScrollOffset.Y - delta.Y, 0.0f);
        }
    }

    Math::Vec2f GuiWindow::GetPos() const
    {
        return m_Pos;
    }

    GuiWindowID GuiWindow::GetID() const
    {
        return m_ID;
    }

    #pragma endregion
}