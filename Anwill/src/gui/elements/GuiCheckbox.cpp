#include "GuiCheckbox.h"
#include "GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiCheckbox::GuiCheckbox(const std::shared_ptr<GuiStyling::Container>& containerStyle, bool checked,
                             const std::string& text, const std::function<void(bool)>& callback,
                             const std::shared_ptr<GuiStyling::Checkbox>& style)
        : GuiElement(containerStyle),
          GuiButton(containerStyle, {containerStyle->elementHeight, containerStyle->elementHeight},
                    [this, callback]() {
              m_Checked = !m_Checked;
              callback(m_Checked);
          }, AW_GUI_MAKE_STYLE(style, GuiStyling::Checkbox)),
          GuiText(containerStyle, text, AW_GUI_CAST_STYLE(GuiButton::m_Style, GuiStyling::Checkbox)),
          m_Style(AW_GUI_CAST_STYLE(GuiButton::m_Style, GuiStyling::Checkbox)),
          m_Checked(checked)
    {
        m_TextPos.X = GuiButton::GetWidth() + GuiStyling::Checkbox::textMargin;
    }

    void GuiCheckbox::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        // Render text
        //Math::Vec2f textPosDelta = {GuiButton::GetWidth() + GuiStyling::Checkbox::textMargin, 0.0f};
        GuiText::Render(assignedPos, assignedMaxSize,
                      delta);

        // Render button
        GuiButton::Render(assignedPos, assignedMaxSize, delta);

        // Render checkmark if it checked
        if(!m_Checked) { return; }
        Math::Vec2f checkmarkMargin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin};
        GuiIcon::RenderIconFunctionType renderFunc = GuiIcon::renderIconFunctions[(size_t) m_Style->checkmarkType];
        renderFunc(assignedPos + Math::Vec2f(checkmarkMargin.X, -checkmarkMargin.Y),
                   GuiButton::GetSize() - checkmarkMargin * 2.0f,
                   m_Style->checkmarkColor);
    }

    bool GuiCheckbox::IsHovering(const Math::Vec2f& mousePos) const
    {
        // TODO: Should hovering the text count as hovering the checkbox?
        return GuiButton::IsHovering(mousePos);
    }

    float GuiCheckbox::GetWidth() const
    {
        return GuiText::GetWidth();
    }

    unsigned int GuiCheckbox::GetGridDepth() const
    {
        return 1;
    }
}