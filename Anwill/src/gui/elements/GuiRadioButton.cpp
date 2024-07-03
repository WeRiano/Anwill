#include "GuiRadioButton.h"
#include "GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiRadioButton::GuiRadioButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                                   const std::string& text, unsigned int textSize, int& reference,
                                   const int onSelectValue, const std::function<void()>& callback,
                                   const std::shared_ptr<GuiStyling::RadioButton>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::RadioButton>() : style),
          m_Text(containerStyle, text, textSize),
          m_Button(containerStyle, {containerStyle->elementHeight, containerStyle->elementHeight},
                   [this, callback](){
                       if(m_Reference != m_OnSelectValue) {
                           callback();
                           m_Reference = m_OnSelectValue;
                       }
                   }),
          m_Reference(reference),
          m_OnSelectValue(onSelectValue)
    {
        m_Button.m_Style->buttonShape = GuiStyling::Button::Shape::Ellipse;
    }

    void GuiRadioButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                                const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        Math::Vec2f textPosDelta = {m_Button.GetWidth() + GuiStyling::Checkbox::textMargin, 0.0f};
        m_Text.Render(assignedPos + textPosDelta, assignedMaxSize - textPosDelta, delta);

        // Render button
        m_Button.Render(assignedPos, assignedMaxSize, delta);

        // Render checkmark if radiobutton is selected
        if(m_Reference != m_OnSelectValue) { return; }
        Math::Vec2f margin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        GuiIcon::RenderEllipse(assignedPos + Math::Vec2f(margin.X, -margin.Y),
                               m_Button.GetSize() - margin * 2.0f,
                               m_Style->checkmarkColor);
    }

    bool GuiRadioButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return m_Button.IsHovering(mousePos);
    }

    float GuiRadioButton::GetWidth() const
    {
        return m_Button.GetWidth() + GuiStyling::Checkbox::textMargin + m_Text.GetWidth();
    }

    unsigned int GuiRadioButton::GetGridDepth() const
    {
        return 1;
    }
}