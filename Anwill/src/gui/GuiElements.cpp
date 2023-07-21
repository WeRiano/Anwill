#include <memory>

#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"
#include "utils/Utils.h"

namespace Anwill {

    #pragma region Helpers

    /**
     * @brief Get the cutoff position of an element given its position and the maximum allowed size
     */
    Math::Vec2f GetCutoffPos(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        return {assignedPos.GetX() + assignedMaxSize.GetX(), assignedPos.GetY() - assignedMaxSize.GetY()};
    }

    /**
     * @brief Get the maximum allowed width and height of an element given its position and
     *        the current maximum width and height.
     */
    Math::Vec2f GetNewMaxSize(const Math::Vec2f& posDelta,
                              const Math::Vec2f& oldMaxSize) {
        return {oldMaxSize.GetX() - posDelta.GetX(), oldMaxSize.GetY() + posDelta.GetY()};
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
                    curPos.GetY() - ((float) currentElementGridDepth * (GuiStyling::Window::elementHeight +
                                                                        GuiStyling::Window::elementVerticalMargin))
            };
        } else {
            return {
                    curPos.GetX() + currentElementWidth + GuiStyling::Window::elementHorizontalMargin,
                    curPos.GetY()
            };
        }
    }

    #pragma endregion

    #pragma region Icon

    void GuiIcon::RenderRightArrow(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec2f& assignedMaxSize,
                                   const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::triangleMesh, iconTransform);
    }

    void GuiIcon::RenderDownArrow(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::triangleMesh, iconTransform);
    }

    void GuiIcon::RenderCross(const Math::Vec2f& assignedPos,
                              const Math::Vec2f& assignedSize,
                              const Math::Vec2f& assignedMaxSize,
                              const Math::Vec3f& color) {
        // TODO
    }

    void GuiIcon::RenderCheckmark(const Math::Vec2f &assignedPos,
                                  const Math::Vec2f &assignedSize,
                                  const Math::Vec2f &assignedMaxSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + (Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::checkmarkMesh, iconTransform);
    }

    void GuiIcon::RenderRectangle(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize, const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.GetX(),
                                                                             -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::rectMesh, iconTransform);
    }

    void GuiIcon::RenderEllipse(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                const Math::Vec2f& assignedMaxSize, const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.GetX(),
                                                                             -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::circleShader->Bind();
        GuiStyling::circleShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::circleShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::circleShader, GuiStyling::rectMesh, iconTransform);
    }

    typedef std::function<void(const Math::Vec2f&, const Math::Vec2f&, const Math::Vec2f&,
                               const Math::Vec3f&)> RenderIconFunction;

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
        // First render the tooltip background window
        Math::Vec2f backgroundTopLeftCorner = mousePos + Math::Vec2f(GuiStyling::Tooltip::offset, 0.0f);
        Math::Vec2f textStartPos = backgroundTopLeftCorner +
                Math::Vec2f(GuiStyling::Tooltip::windowMargin.GetX(),
                            -GuiStyling::Tooltip::windowMargin.GetY()) +
                            Math::Vec2f(0.0f, -GuiStyling::Text::font->GetFontHeight())
                            * m_TooltipTextScale;

        Math::Vec2f textSize = GuiStyling::Text::font->GetTextSize(m_TooltipText) * m_TooltipTextScale;
        Math::Vec2f backgroundSize = textSize + GuiStyling::Tooltip::windowMargin * 2.0f;

        // Move to fit inside game window
        bool isOutsideX = backgroundTopLeftCorner.GetX() + backgroundSize.GetX() > gameWindowSize.GetX();
        bool isOutsideY = backgroundTopLeftCorner.GetY() - backgroundSize.GetY() < 0.0f;
        Math::Vec2f correctedOffset;
        if(isOutsideX and isOutsideY) {
            float correctedOffsetX = gameWindowSize.GetX() -
                                     (backgroundTopLeftCorner.GetX() + backgroundSize.GetX());
            correctedOffset = {correctedOffsetX, GuiStyling::Tooltip::offset + backgroundSize.GetY()};
        } else if (isOutsideX) {
            float correctedOffsetX = gameWindowSize.GetX() -
                    (backgroundTopLeftCorner.GetX() + backgroundSize.GetX());
            correctedOffset = {correctedOffsetX, -GuiStyling::Tooltip::offset};
        } else if (isOutsideY) {
            float correctedOffsetY = -(backgroundTopLeftCorner.GetY() - backgroundSize.GetY());
            correctedOffset = {0.0f, correctedOffsetY};
        }

        Math::Mat4f transform = Math::Mat4f::Scale({}, backgroundSize);
        transform = Math::Mat4f::Translate(transform, backgroundTopLeftCorner + correctedOffset +
                                Math::Vec2f(backgroundSize.GetX(), -backgroundSize.GetY()) * 0.5f);
        Renderer2D::Submit(GuiStyling::Tooltip::shader, GuiStyling::rectMesh, transform);

        // Then render the tooltip text
        transform = Math::Mat4f::Scale({}, {m_TooltipTextScale, m_TooltipTextScale, 0.0f});
        transform = Math::Mat4f::Translate(transform, textStartPos + correctedOffset);
        GuiStyling::Text::shader->Bind();
        GuiStyling::Text::shader->SetUniformVec2f({gameWindowSize.GetX(), -gameWindowSize.GetY()},
                                                "u_CutoffPos");
        GuiStyling::Text::shader->Unbind();
        Renderer2D::Submit(GuiStyling::Text::shader, *GuiStyling::Text::font, m_TooltipText, transform);
    }

    #pragma endregion

    #pragma region Element

    GuiElement::GuiElement()
        : m_IsHovered(false), m_IsPressed(false)
    {}

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::OnHover(const Math::Vec2f& mousePos)
    {
        // Default behavior is nothing
    }

    void GuiElement::StartPressing()
    {
        m_IsPressed = true;
    }

    void GuiElement::OnPress(const Math::Vec2f& mousePos)
    {
        // Default behavior is nothing
    }

    void GuiElement::Release()
    {
        m_IsPressed = false;
    }

    void GuiElement::Select()
    {
        AW_INFO("Element selected!");
        m_IsSelected = true;
    }

    void GuiElement::Deselect()
    {
        AW_INFO("Element deslected!");
        m_IsSelected = false;
    }

    void GuiElement::OnKeyPress()
    {

    }

    void GuiElement::OnKeyRepeat()
    {

    }

    void GuiElement::OnKeyRelease()
    {

    }

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

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = GetCutoffPos(Math::Vec2f(m_TextPos.GetX(), 0.0f) + assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, m_TextPos + assignedPos);

        // Render
        GuiStyling::Text::shader->Bind();
        GuiStyling::Text::shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiStyling::Text::shader, *GuiStyling::Text::font, m_Text, thisTransform);
    }

    bool GuiText::IsHovering(const Math::Vec2f& mousePos) const {
        return false;
    }

    float GuiText::GetWidth() const
    {
        return m_TextWidth;
    }

    unsigned int GuiText::GetGridDepth() const
    {
        return 1;
    }

    void GuiText::SetText(const std::string& text) {
        m_Text = text;
        m_TextWidth = (float) GuiStyling::Text::font->GetTextWidth(text) * m_TextScale;
    }

    #pragma endregion

    #pragma region Button

    GuiButton::GuiButton(const Math::Vec2f& size, const std::function<void()>& callback)
        : GuiElement(),
          m_ButtonSize(size),
          m_Callback(callback)
    {}

    void GuiButton::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_ButtonSize);
        thisTransform = Math::Mat4f::Translate(thisTransform,
                                               assignedPos + Math::Vec2f(m_ButtonSize.GetX() / 2.0f,
                                                                         -m_ButtonSize.GetY() / 2.0f));

        // Render button
        auto shader = GuiStyling::Button::shaders[(std::size_t) m_ButtonStyle.buttonShape];
        shader->Bind();
        shader->SetUniform1i(m_IsHovered, "u_Hovering");
        shader->SetUniform1i(m_IsPressed, "u_Pressing");
        shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        shader->SetUniformVec3f(m_ButtonStyle.buttonColor, "u_Color");
        shader->SetUniformVec3f(m_ButtonStyle.buttonHoverColor, "u_HoverColor");
        shader->SetUniformVec3f(m_ButtonStyle.buttonPressColor, "u_PressColor");
        Renderer2D::Submit(shader, GuiStyling::rectMesh, thisTransform);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return Math::Algo::IsPointInsideRectangle({0.0f,                 0.0f},
                                                  {m_ButtonSize.GetX(),  0.0f},
                                                  {m_ButtonSize.GetX(), -m_ButtonSize.GetY()},
                                                  {0.0f,                -m_ButtonSize.GetY()},
                                                  mousePos);
    }

    float GuiButton::GetWidth() const
    {
        return m_ButtonSize.GetX();
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

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render text
        Math::Vec2f padding = Math::Vec2f(GuiStyling::TextButton::textPadding, 0.0f);
        m_Text.Render(assignedPos + padding,
                      assignedMaxSize - padding);
    }

    void GuiTextButton::SetText(const std::string& text) {
        m_Text.SetText(text);
        m_ButtonSize = { m_Text.GetWidth() + GuiStyling::TextButton::textPadding * 2.0f, m_ButtonSize.GetY() };
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

    void GuiCheckbox::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        // Render text
        Math::Vec2f textPosDelta = {m_ButtonSize.GetX() + GuiStyling::Checkbox::textMargin, 0.0f};
        m_Text.Render(assignedPos + textPosDelta, assignedMaxSize - textPosDelta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render checkmark if it checked
        if(!m_Checked) { return; }
        Math::Vec2f margin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        RenderIconFunction renderFunc = renderIconFunctions[(size_t) m_CheckboxStyle.checkmarkType];
        renderFunc(assignedPos + Math::Vec2f(margin.GetX(), -margin.GetY()),
                                 m_ButtonSize - margin * 2.0f,
                                 assignedMaxSize - margin,
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

    void GuiRadioButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f textPosDelta = {m_ButtonSize.GetX() + GuiStyling::Checkbox::textMargin, 0.0f};
        m_Text.Render(assignedPos + textPosDelta, assignedMaxSize - textPosDelta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render checkmark if radiobutton is selected
        if(m_Reference != m_OnSelectValue) { return; }
        Math::Vec2f margin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        GuiIcon::RenderEllipse(assignedPos + Math::Vec2f(margin.GetX(), -margin.GetY()),
                   m_ButtonSize - margin * 2.0f,
                   assignedMaxSize - margin,
                   m_RadioButtonStyle.checkmarkColor);
    }

    float GuiRadioButton::GetWidth() const
    {
        return GuiButton::GetWidth() + GuiStyling::Checkbox::textMargin + m_Text.GetWidth();
    }

    #pragma endregion

    #pragma region Slider

    GuiSlider::GuiSlider()
        : GuiButton({GuiStyling::Window::elementHeight * 7.0f, GuiStyling::Window::elementHeight}, [](){}),
          m_ValueText("", GuiStyling::Text::fontSize)
    {}

    void GuiSlider::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        // Render background button
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render "slider" (marker) rectangle
        Math::Vec2f markerPos = {m_LastCursorXPos - GuiStyling::Slider::markerSize.GetX() * 0.5f,
                                 -(m_ButtonSize.GetY() - GuiStyling::Slider::markerSize.GetY()) * 0.5f + 1.0f};
        markerPos = {Utils::Clamp(markerPos.GetX(), 0.0f, GetWidth() - GuiStyling::Slider::markerSize.GetX()),
                     markerPos.GetY()};
        GuiIcon::RenderRectangle(assignedPos + markerPos, GuiStyling::Slider::markerSize,
                                 assignedMaxSize - markerPos, m_SliderStyle.markerColor);

        // Render text
        float centeredTextXPos = m_ButtonSize.GetX() * 0.5f - m_ValueText.GetWidth() * 0.5f;
        m_ValueText.Render(assignedPos + Math::Vec2f(centeredTextXPos, 0.0f),
                      assignedMaxSize - Math::Vec2f(centeredTextXPos + GuiStyling::TextButton::textPadding, 0.0f));
    }

    void GuiSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiElement::OnPress(mousePos);

        m_LastCursorXPos = mousePos.GetX();
    }

    #pragma endregion

    #pragma region IntSlider

    GuiIntSlider::GuiIntSlider(int min, int max, int* sliderValue)
        : m_Min(min), m_Max(max), m_ClientValuePointer(sliderValue)
    {
        int startValue = m_Min;
        m_ValueText.SetText(std::to_string(startValue));
        m_LastCursorXPos = 0.0f;
    }

    void GuiIntSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiSlider::OnPress(mousePos);

        m_LastCursorXPos = mousePos.GetX();
        float sliderValue = Utils::ScaleToRange(mousePos.GetX(),
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                GuiStyling::Slider::markerSize.GetX() * 0.5f,
                                                GetWidth() - GuiStyling::Slider::markerSize.GetX() * 0.5f);
        sliderValue = Utils::Clamp(sliderValue,
                                   static_cast<float>(m_Min),
                                   static_cast<float>(m_Max));
        int roundedInt = Utils::RoundToInt(sliderValue);
        *m_ClientValuePointer = roundedInt;
        m_ValueText.SetText(std::to_string(roundedInt));
    }

    #pragma endregion

    #pragma region FloatSlider

    GuiFloatSlider::GuiFloatSlider(float min, float max, float* sliderValue)
        : m_Min(min), m_Max(max), m_ClientValuePointer(sliderValue)
    {
        float startValue = m_Min;
        m_ValueText.SetText(Utils::RoundFloatToString(startValue, 3));
        m_LastCursorXPos = 0.0f;
    }

    void GuiFloatSlider::OnPress(const Math::Vec2f& mousePos) {
        GuiSlider::OnPress(mousePos);

        float sliderValue = Utils::ScaleToRange(mousePos.GetX(),
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                GuiStyling::Slider::markerSize.GetX() * 0.5f,
                                                GetWidth() - GuiStyling::Slider::markerSize.GetX() * 0.5f);
        sliderValue = Utils::Clamp(sliderValue,
                                   static_cast<float>(m_Min),
                                   static_cast<float>(m_Max));
        *m_ClientValuePointer = sliderValue;
        m_ValueText.SetText(Utils::RoundFloatToString(sliderValue, 3));
    }

    #pragma endregion

    #pragma region InputText

    GuiInputText::GuiInputText(const std::string& startText, unsigned int textSize, float pixelWidth)
        : GuiTextButton(startText, textSize, [](){}), m_Selected(false)
    {
        m_ButtonSize = {pixelWidth, m_ButtonSize.GetY()};
        m_ButtonStyle.buttonHoverColor = m_ButtonStyle.buttonColor;
        m_ButtonStyle.buttonPressColor = m_ButtonStyle.buttonColor;
    }

    void GuiInputText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        GuiTextButton::Render(assignedPos, assignedMaxSize);

        if(!m_Selected) { return; }
        // TODO: Render "editing bar" (this guy: | )
    }

    void GuiInputText::OnHover(const Math::Vec2f& mousePos)
    {

    }

    void GuiInputText::OnPress(const Math::Vec2f& mousePos)
    {
        GuiElement::OnPress(mousePos);
    }

    #pragma endregion

    #pragma region Container

    GuiContainer::GuiContainer()
            : m_GridDepth(1), m_HideElements(true)
    {}

    std::shared_ptr<GuiElement> GuiContainer::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                              const Math::Vec2f& mousePos) const
    {
        if(m_HideElements) { return nullptr; }
        for(unsigned int i = 0; i < m_Elements.size(); i++) {
            auto& element = m_Elements[i];
            if (element->IsHovering(mousePos - (m_ElementPosCache[i]))) {
                hoverElementPos = m_ElementPosCache[i];
                return element;
            }
            if (dynamic_cast<GuiContainer*>(element.get()) != nullptr) {
                // If element is a container we need to check with those elements
                auto container = std::dynamic_pointer_cast<GuiContainer>(element);
                Math::Vec2f elementPosInsideContainer = {};
                auto maybeResult = container->GetHoverElement(
                        elementPosInsideContainer,
                        mousePos - (m_ElementPosCache[i]));
                if (maybeResult != nullptr) {
                    // If we found an element we return it, otherwise we continue
                    hoverElementPos = m_ElementPosCache[i] + elementPosInsideContainer;
                    return maybeResult;
                }
            }
        }
        return nullptr;
    }

    void GuiContainer::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Math::Vec2f& firstPos)
    {
        m_ElementPosCache.clear();
        Math::Vec2f elementGridPos = firstPos;
        float newRowXPos = firstPos.GetX();
        std::shared_ptr<GuiElement> lastElement = nullptr;
        for (unsigned int i = 0; i < m_Elements.size(); i++) {
            auto element = m_Elements[i];
            if(i > 0) {
                bool wantsNewRow = m_NewRowChecks[i].first;
                bool forcedToNewRow = m_NewRowChecks[i].second;
                elementGridPos = GetNextElementPos(elementGridPos, lastElement->GetWidth(),
                                                   lastElement->GetGridDepth(),
                                                   newRowXPos,
                                                   wantsNewRow || forcedToNewRow);
            }
            element->Render(assignedPos + elementGridPos,
                            GetNewMaxSize(elementGridPos + Math::Vec2f(GuiStyling::Window::cutoffPadding,
                                                                       -GuiStyling::Window::cutoffPadding),
                                          assignedMaxSize));
            m_ElementPosCache.push_back(elementGridPos);
            lastElement = element;
        }
    }

    bool GuiContainer::IsHidingElements() const
    {
        return m_HideElements;
    }

    #pragma endregion

    #pragma region Dropdown

    GuiDropdown::GuiDropdown(const std::string& text, unsigned int textSize)
            : GuiTextButton(text, textSize, [this](){
        m_HideElements = !m_HideElements;
    })
    {}

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        // Force button size to max width render it
        m_ButtonSize = { assignedMaxSize.GetX(), m_ButtonSize.GetY()};
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render arrow icon
        if(m_HideElements) {
            GuiIcon::RenderRightArrow(assignedPos,
                                      GuiStyling::iconSize * 0.5f,
                                      assignedMaxSize,
                                      GuiStyling::iconColor);
        } else {
            GuiIcon::RenderDownArrow(assignedPos,
                                     GuiStyling::iconSize * 0.5f,
                                     assignedMaxSize,
                                     GuiStyling::iconColor);
        }

        // Render text slightly to the right compared to a regular text button
        m_Text.Render(assignedPos + Math::Vec2f(GuiStyling::iconSize.GetX(), 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiStyling::iconSize.GetX(), 0.0f));

        if(m_HideElements) { return; }
        GuiContainer::Render(assignedPos, assignedMaxSize,
                             { GuiStyling::Dropdown::elementIndent,
                               -GuiStyling::Window::elementHeight - GuiStyling::Window::elementVerticalMargin });
    }

    bool GuiDropdown::IsHovering(const Math::Vec2f& mousePos) const
    {
        if (GuiTextButton::IsHovering(mousePos)) {
            return true;
        } else {
            for(unsigned int i = 0; i < m_Elements.size(); i++) {
                if(m_Elements[i]->IsHovering(mousePos)) {
                    return true;
                }
            }
        }
        return false;
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
                                                                   m_Size = {m_Size.GetX(),
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
        //  1. We have a static minimize button element that we have to consider
        //  2. We have to consider the position of the window since it is the "outer" most container
        if(m_MinimizeButton->IsHovering(mousePos - m_Pos)) {
            return m_MinimizeButton;
        }
        auto hoverElement = GuiContainer::GetHoverElement(hoverElementPos, mousePos - m_Pos);
        hoverElementPos = hoverElementPos + m_Pos;
        return hoverElement;
    }

    void GuiWindow::Render(bool selected)
    {
        // Render window
        GuiStyling::Window::shader->Bind();
        GuiStyling::Window::shader->SetUniform1i(selected, "u_Selected");
        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(),
                                                m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::Submit(GuiStyling::Window::shader, GuiStyling::rectMesh, transform);

        // Render title
        m_Title.Render(m_Pos + GuiStyling::Window::titlePos, m_Size - GuiStyling::Window::titlePos
                                                           - Math::Vec2f(GuiStyling::Window::cutoffPadding, GuiStyling::Window::cutoffPadding));

        // Render minimize button
        m_MinimizeButton->Render(m_Pos, m_Size);
        if(m_HideElements) {
            GuiIcon::RenderRightArrow(m_Pos,
                                      GuiStyling::iconSize * 0.5f,
                                      m_Size - Math::Vec2f(GuiStyling::Window::cutoffPadding,
                                                           GuiStyling::Window::cutoffPadding),
                                      GuiStyling::iconColor);
            return;
        } else {
            GuiIcon::RenderDownArrow(m_Pos,
                                     GuiStyling::iconSize * 0.5f,
                                     m_Size - Math::Vec2f(GuiStyling::Window::cutoffPadding, GuiStyling::Window::cutoffPadding),
                                     GuiStyling::iconColor);
        }


        GuiContainer::Render(m_Pos, m_Size,
                             {GuiStyling::Window::elementIndent,
                              -(GuiStyling::Window::headerSize + GuiStyling::Window::elementVerticalMargin) });
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX() + GuiStyling::Window::titlePos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - GuiStyling::Window::headerSize},
                                                  {m_Pos.GetX() + GuiStyling::Window::titlePos.GetX(), m_Pos.GetY() - GuiStyling::Window::headerSize},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringResize(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideCircle({m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                               10.0f, mousePos);
    }

    bool GuiWindow::IsHoveringWindow(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  {m_Pos.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  mousePos);
    }

    bool GuiWindow::IsHoveringMinimize(const Math::Vec2f& mousePos) {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + GuiStyling::iconSize.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + GuiStyling::iconSize.GetX(), m_Pos.GetY()
                                                  - GuiStyling::Window::headerSize},
                                                  {m_Pos.GetX(), m_Pos.GetY() - GuiStyling::Window::headerSize},
                                                  mousePos);
    }

    void GuiWindow::Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos)
    {
        if(!(m_Pos.GetY() + delta.GetY() > maxPos.GetY() ||
             m_Pos.GetY() - GuiStyling::Window::headerSize + delta.GetY() < minPos.GetY())) {
            m_Pos = {m_Pos.GetX(), m_Pos.GetY() + delta.GetY()};
        }
        if(!(m_Pos.GetX() + delta.GetX() < minPos.GetX() ||
             m_Pos.GetX() + m_Size.GetX() + delta.GetX() > maxPos.GetX())) {
            m_Pos = {m_Pos.GetX() + delta.GetX(), m_Pos.GetY()};
        }
    }

    void GuiWindow::Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize)
    {
        Math::Vec2f newSize = m_Size + delta;
        newSize.Clamp(minSize, maxSize);
        m_Size = newSize;
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