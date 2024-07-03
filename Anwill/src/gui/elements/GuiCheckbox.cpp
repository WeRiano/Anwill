#include "GuiCheckbox.h"
#include "GuiIcon.h"
#include "utils/Profiler.h"

namespace Anwill {

    GuiCheckbox::GuiCheckbox(const std::shared_ptr<GuiStyling::Container>& containerStyle, bool checked,
                             const std::string& text, unsigned int textSize, const std::function<void(bool)>& callback,
                             const std::shared_ptr<GuiStyling::Checkbox>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::Checkbox>() : style),
          m_Text(containerStyle, text, textSize, style),
          m_Button(containerStyle, {containerStyle->elementHeight, containerStyle->elementHeight},
                   [this, callback](){
                       m_Checked = !m_Checked;
                       callback(m_Checked);
                   }, style),
          m_Checked(checked)
    {}

    void GuiCheckbox::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                             const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        // Render text
        Math::Vec2f textPosDelta = {m_Button.GetWidth() + GuiStyling::Checkbox::textMargin, 0.0f};
        m_Text.Render(assignedPos + textPosDelta, assignedMaxSize - textPosDelta,
                      delta);

        // Render button
        m_Button.Render(assignedPos, assignedMaxSize, delta);

        // Render checkmark if it checked
        if(!m_Checked) { return; }
        Math::Vec2f margin = {GuiStyling::Checkbox::iconMargin, GuiStyling::Checkbox::iconMargin * 1.0f};
        GuiIcon::RenderIconFunctionType renderFunc = GuiIcon::renderIconFunctions[(size_t) m_Style->checkmarkType];
        renderFunc(assignedPos + Math::Vec2f(margin.X, -margin.Y),
                   m_Button.GetSize() - margin * 2.0f,
                   m_Style->checkmarkColor);
    }

    bool GuiCheckbox::IsHovering(const Math::Vec2f& mousePos) const
    {
        return m_Button.IsHovering(mousePos);
    }

    float GuiCheckbox::GetWidth() const
    {
        return m_Button.GetWidth() + GuiStyling::Checkbox::textMargin + m_Text.GetWidth();
    }

    unsigned int GuiCheckbox::GetGridDepth() const
    {
        return 1;
    }
}