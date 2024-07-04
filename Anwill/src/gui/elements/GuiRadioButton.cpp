#include "GuiRadioButton.h"
#include "GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiRadioButton::GuiRadioButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                                   const std::string& text, int& reference, const int onSelectValue,
                                   const std::function<void()>& callback,
                                   const std::shared_ptr<GuiStyling::RadioButton>& style)
        : GuiElement(containerStyle),
          GuiButton(containerStyle, {containerStyle->elementHeight, containerStyle->elementHeight},
                    [this, callback](){
                        if(m_Reference != m_OnSelectValue) {
                            callback();
                            m_Reference = m_OnSelectValue;
                        }
                    }, AW_GUI_MAKE_STYLE(style, GuiStyling::RadioButton)),
          GuiText(containerStyle, text, AW_GUI_CAST_STYLE(GuiButton::m_Style, GuiStyling::RadioButton)),
          m_Style(AW_GUI_CAST_STYLE(GuiButton::m_Style, GuiStyling::RadioButton)),
          m_Reference(reference),
          m_OnSelectValue(onSelectValue)
    {
        GuiButton::m_Style->buttonShape = GuiStyling::Button::Shape::Ellipse;
        m_TextPos.X = GuiButton::GetWidth() + GuiStyling::Checkbox::textMargin;
    }

    void GuiRadioButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                                const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        GuiText::Render(assignedPos, assignedMaxSize, delta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        // Render checkmark if radiobutton is selected
        if(m_Reference != m_OnSelectValue) { return; }
        Math::Vec2f checkmarkMargin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        GuiIcon::RenderEllipse(assignedPos + Math::Vec2f(checkmarkMargin.X, -checkmarkMargin.Y),
                               GuiButton::GetSize() - checkmarkMargin * 2.0f,
                               m_Style->checkmarkColor);
    }

    bool GuiRadioButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        // TODO: Should hovering the text count as hovering the radio button?
        return GuiButton::IsHovering(mousePos);
    }

    float GuiRadioButton::GetWidth() const
    {
        return GuiText::GetWidth();
    }

    unsigned int GuiRadioButton::GetGridDepth() const
    {
        return 1;
    }
}